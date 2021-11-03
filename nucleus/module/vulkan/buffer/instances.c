#include <nucleus/module/vulkan/buffer/instances.h>

#define INSTANCE_HEADER_SIZE (sizeof(nu_vec4f_t) * 3 + sizeof(nu_vec4f_t) + sizeof(uint32_t) * 4)

nu_result_t nuvk_buffer_instances_create(
    nuvk_buffer_instances_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
)
{
    nu_result_t result;

    /* index buffer */
    buffer->index_uniform_buffer_range = NU_MIN(context->physical_device_properties.limits.maxUniformBufferRange, 1 << 16);
    buffer->next_index_offset = 0;
    buffer->index_offsets = (uint32_t*)nu_malloc(sizeof(uint32_t) * NUVK_MAX_SDF_COUNT);

    nuvk_buffer_info_t index_buffer_info;
    memset(&index_buffer_info, 0, sizeof(nuvk_buffer_info_t));
    index_buffer_info.size         = buffer->index_uniform_buffer_range * render_context->max_inflight_frame_count;
    index_buffer_info.buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    index_buffer_info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    result = nuvk_dynamic_range_buffer_create(
        &buffer->index_buffer, memory_manager,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
        buffer->index_uniform_buffer_range,
        render_context->max_inflight_frame_count,
        128
    );
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create index buffer.");

    /* instances buffer */
    buffer->instance_uniform_buffer_range = NU_MIN(context->physical_device_properties.limits.maxUniformBufferRange, 1 << 16);
    buffer->next_instance_offset = 0;
    buffer->instance_offsets = (uint32_t*)nu_malloc(sizeof(uint32_t) * NUVK_MAX_SDF_COUNT);
    buffer->instance_sizes   = (uint32_t*)nu_malloc(sizeof(uint32_t) * NUVK_MAX_SDF_COUNT);

    result = nuvk_dynamic_range_buffer_create(
        &buffer->instance_buffer, memory_manager,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
        buffer->instance_uniform_buffer_range,
        render_context->max_inflight_frame_count,
        128
    );
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create instance buffer.");

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_instances_destroy(
    nuvk_buffer_instances_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_dynamic_range_buffer_destroy(&buffer->index_buffer, memory_manager);
    nu_free(buffer->index_offsets);
    nuvk_dynamic_range_buffer_destroy(&buffer->instance_buffer, memory_manager);
    nu_free(buffer->instance_offsets);
    nu_free(buffer->instance_sizes);

    return NU_SUCCESS;
}

nu_result_t nuvk_buffer_instances_configure_sdfs(
    nuvk_buffer_instances_t *buffer,
    nuvk_sdf_data_t **sdfs,
    uint32_t sdf_count
)
{
    NU_CHECK(sdf_count < NUVK_MAX_SDF_COUNT, return NU_FAILURE, NUVK_LOGGER_NAME, "Max sdf count reached.");

    buffer->next_index_offset    = 0;
    buffer->next_instance_offset = 0;
    for (uint32_t i = 0; i < sdf_count; i++) {
        buffer->instance_sizes[i]     = (INSTANCE_HEADER_SIZE + sdfs[i]->data_size); /* must have 16 bytes alignment */
        buffer->instance_offsets[i]   = buffer->next_instance_offset;
        buffer->next_instance_offset += sdfs[i]->max_instance_count * buffer->instance_sizes[i];

        NU_CHECK(buffer->next_instance_offset < buffer->instance_uniform_buffer_range, return NU_FAILURE,
            NUVK_LOGGER_NAME, "Max instance uniform buffer range reached.");

        buffer->index_offsets[i]   = buffer->next_index_offset;
        buffer->next_index_offset += sizeof(uint32_t) * 4 * sdfs[i]->max_instance_count;

        NU_CHECK(buffer->next_index_offset < buffer->index_uniform_buffer_range, return NU_FAILURE,
            NUVK_LOGGER_NAME, "Max index uniform buffer range reached.");

        /* save buffer id */
        sdfs[i]->buffer_id = i;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_instances_write_indices(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t *indices,
    uint32_t index_count
)
{
    NU_ASSERT(index_count < NUVK_MAX_SDF_COUNT);

    const uint32_t end_value = (uint32_t)0xFFFFFFFF;

    if (index_count > 0) {
        uint32_t buf[4 * (index_count + 1)];
        memset(buf, 0, sizeof(uint32_t) * 4 * index_count);
        for (uint32_t i = 0; i < index_count; i++) {
            buf[i * 4] = indices[i];
        }
        buf[4 * index_count] = end_value;

        nuvk_dynamic_range_buffer_write(&buffer->index_buffer, active_inflight_frame_index,
            buffer->index_offsets[sdf_index], sizeof(uint32_t) * 4 * (index_count + 1), buf);
    } else {
        nuvk_dynamic_range_buffer_write(&buffer->index_buffer, active_inflight_frame_index,
            buffer->index_offsets[sdf_index], sizeof(uint32_t), &end_value);
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_instances_write_instance_transform(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t instance_index,
    const nu_mat3f_t inv_rotation,
    const nu_vec4f_t translation_scale
)
{
    NU_ASSERT(sdf_index < NUVK_MAX_SDF_COUNT);

    uint32_t offset = buffer->instance_offsets[sdf_index] + buffer->instance_sizes[sdf_index] * instance_index;

    uint8_t buf[INSTANCE_HEADER_SIZE];
    memcpy(buf + sizeof(nu_vec4f_t) * 0, inv_rotation[0], sizeof(nu_vec3f_t));
    memcpy(buf + sizeof(nu_vec4f_t) * 1, inv_rotation[1], sizeof(nu_vec3f_t));
    memcpy(buf + sizeof(nu_vec4f_t) * 2, inv_rotation[2], sizeof(nu_vec3f_t));
    memcpy(buf + sizeof(nu_vec4f_t) * 3, translation_scale, sizeof(nu_vec4f_t));

    nuvk_dynamic_range_buffer_write(&buffer->instance_buffer, active_inflight_frame_index,
        offset, INSTANCE_HEADER_SIZE, buf);

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_instances_write_instance_data(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t instance_index,
    const void *data
)
{
    NU_ASSERT(sdf_index < NUVK_MAX_SDF_COUNT);

    uint32_t offset = buffer->instance_offsets[sdf_index]
        + buffer->instance_sizes[sdf_index] * instance_index
        + INSTANCE_HEADER_SIZE;

    uint32_t data_size = (buffer->instance_sizes[sdf_index] - INSTANCE_HEADER_SIZE);

    nuvk_dynamic_range_buffer_write(&buffer->instance_buffer, active_inflight_frame_index,
        offset, data_size, data);

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_instances_write_instance_material(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t instance_index,
    uint32_t material_index
)
{
    NU_ASSERT(sdf_index < NUVK_MAX_SDF_COUNT);

    uint32_t offset = buffer->instance_offsets[sdf_index]
        + buffer->instance_sizes[sdf_index] * instance_index
        + sizeof(nu_vec4f_t) * 3 + sizeof(nu_vec4f_t);

    nuvk_dynamic_range_buffer_write(&buffer->instance_buffer, active_inflight_frame_index,
        offset, sizeof(uint32_t), &material_index);

    return NU_SUCCESS;
}