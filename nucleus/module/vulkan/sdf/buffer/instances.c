#include <nucleus/module/vulkan/sdf/buffer/instances.h>

#define INSTANCE_HEADER_SIZE (sizeof(nu_vec4f_t) * 3 + sizeof(nu_vec4f_t))

nu_result_t nuvk_sdf_buffer_instances_create(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
)
{
    nu_result_t result;

    /* index buffer */
    buffer->index_uniform_buffer_range = NU_MIN(context->physical_device_properties.limits.maxUniformBufferRange, 1 << 16);
    buffer->next_index_offset = 0;
    buffer->index_offsets = (uint32_t*)nu_malloc(sizeof(uint32_t) * NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);

    nuvk_buffer_info_t index_buffer_info;
    memset(&index_buffer_info, 0, sizeof(nuvk_buffer_info_t));
    index_buffer_info.size         = buffer->index_uniform_buffer_range * render_context->max_inflight_frame_count;
    index_buffer_info.buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    index_buffer_info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    result = nuvk_buffer_create(&buffer->index_buffer, memory_manager, &index_buffer_info);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create index buffer.");

    result = nuvk_buffer_map(&buffer->index_buffer, memory_manager);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to map index buffer.");
 
    /* instances buffer */
    buffer->instance_uniform_buffer_range = NU_MIN(context->physical_device_properties.limits.maxUniformBufferRange, 1 << 16);
    buffer->next_instance_offset = 0;
    buffer->instance_offsets = (uint32_t*)nu_malloc(sizeof(uint32_t) * NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);
    buffer->instance_sizes   = (uint32_t*)nu_malloc(sizeof(uint32_t) * NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);

    nuvk_buffer_info_t instance_buffer_info;
    memset(&instance_buffer_info, 0, sizeof(nuvk_buffer_info_t));
    instance_buffer_info.size         = buffer->instance_uniform_buffer_range * render_context->max_inflight_frame_count;
    instance_buffer_info.buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    instance_buffer_info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    result = nuvk_buffer_create(&buffer->instance_buffer, memory_manager, &instance_buffer_info);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create instance buffer.");

    result = nuvk_buffer_map(&buffer->instance_buffer, memory_manager);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to map instance buffer.");

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_instances_destroy(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_buffer_unmap(&buffer->index_buffer, memory_manager);
    nuvk_buffer_destroy(&buffer->index_buffer, memory_manager);
    nu_free(buffer->index_offsets);
    nuvk_buffer_unmap(&buffer->instance_buffer, memory_manager);
    nuvk_buffer_destroy(&buffer->instance_buffer, memory_manager);
    nu_free(buffer->instance_offsets);
    nu_free(buffer->instance_sizes);

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_buffer_instances_configure_instance_types(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_sdf_instance_type_info_t *info,
    uint32_t type_count
)
{
    NU_CHECK(type_count < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT, return NU_FAILURE, NUVK_LOGGER_NAME, "Max instance type count reached.");

    buffer->next_index_offset    = 0;
    buffer->next_instance_offset = 0;
    for (uint32_t i = 0; i < type_count; i++) {
        buffer->instance_sizes[i]     = (INSTANCE_HEADER_SIZE + info[i].data_size); /* must have 16 bytes alignment */
        buffer->instance_offsets[i]   = buffer->next_instance_offset;
        buffer->next_instance_offset += info[i].max_instance_count * buffer->instance_sizes[i];

        NU_CHECK(buffer->next_instance_offset < buffer->instance_uniform_buffer_range, return NU_FAILURE,
            NUVK_LOGGER_NAME, "Max instance uniform buffer range reached.");

        buffer->index_offsets[i]   = buffer->next_index_offset;
        buffer->next_index_offset += sizeof(uint32_t) * 4 * (info[i].max_instance_count + 1); /* +1 with the index count */

        NU_CHECK(buffer->next_index_offset < buffer->index_uniform_buffer_range, return NU_FAILURE,
            NUVK_LOGGER_NAME, "Max index uniform buffer range reached.");
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_instances_write_index_count(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t count
)
{
    NU_ASSERT(type_index < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);

    char *data = (char*)buffer->index_buffer.map + buffer->index_uniform_buffer_range * active_inflight_frame_index
        + buffer->index_offsets[type_index];
    *((uint32_t*)data) = count;

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_instances_write_index(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t index_position,
    uint32_t index
)
{
    NU_ASSERT(type_index < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);

    char *data = (char*)buffer->index_buffer.map + buffer->index_uniform_buffer_range * active_inflight_frame_index
        + buffer->index_offsets[type_index] 
        + sizeof(uint32_t) * 4 * (index_position + 1); /* using std140 */
    *((uint32_t*)data) = index;

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_instances_write_instance_transform(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t instance_index,
    const nu_mat3f_t inv_rotation,
    const nu_vec4f_t translation_scale
)
{
    NU_ASSERT(type_index < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);

    char *data = (char*)buffer->instance_buffer.map + (buffer->instance_uniform_buffer_range * active_inflight_frame_index)
        + buffer->instance_offsets[type_index]
        + buffer->instance_sizes[type_index] * instance_index;

    memcpy(data + sizeof(nu_vec4f_t) * 0, inv_rotation[0], sizeof(nu_vec3f_t));
    memcpy(data + sizeof(nu_vec4f_t) * 1, inv_rotation[1], sizeof(nu_vec3f_t));
    memcpy(data + sizeof(nu_vec4f_t) * 2, inv_rotation[2], sizeof(nu_vec3f_t));
    memcpy(data + sizeof(nu_vec4f_t) * 3, translation_scale, sizeof(nu_vec4f_t));

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_instances_write_instance_data(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t instance_index,
    void *data
)
{
    NU_ASSERT(type_index < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT);

    char *pdata = (char*)buffer->instance_buffer.map + (buffer->instance_uniform_buffer_range * active_inflight_frame_index)
        + buffer->instance_offsets[type_index]
        + buffer->instance_sizes[type_index] * instance_index
        + INSTANCE_HEADER_SIZE;
    uint32_t data_size = (buffer->instance_sizes[type_index] - INSTANCE_HEADER_SIZE);
    memcpy(pdata, data, data_size);

    return NU_SUCCESS;
}