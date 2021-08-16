#include <nucleus/module/vulkan/sdf/buffer/instances.h>

static void nuvk_sdf_buffer_instances_write_instance_count(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t count
)
{
    uint32_t *data = (uint32_t*)((char*)buffer->header_buffer.map + buffer->total_header_buffer_size * active_inflight_frame_index);
    data[type_index] = count;
}

static void nuvk_sdf_buffer_instances_write_instance_matrix(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t instance_index,
    const nu_mat4f_t matrix
)
{
    void *pdata = (char*)buffer->type_arrays_buffer.map + (buffer->total_type_arrays_buffer_size * active_inflight_frame_index)
        + buffer->type_arrays_offsets[type_index]
        + buffer->type_arrays_item_sizes[type_index] * instance_index; 
    nu_mat4f_copy(matrix, (nu_vec4f_t*)pdata);
}

static void nuvk_sdf_buffer_instances_write_instance_data(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t instance_index,
    void *data
)
{
    void *pdata = (char*)buffer->type_arrays_buffer.map + (buffer->total_type_arrays_buffer_size * active_inflight_frame_index)
        + buffer->type_arrays_offsets[type_index]
        + buffer->type_arrays_item_sizes[type_index] * instance_index
        + sizeof(nu_mat4f_t); 
    uint32_t data_size = (buffer->type_arrays_item_sizes[type_index] - sizeof(nu_mat4f_t));
    memcpy(pdata, data, data_size);
}

nu_result_t nuvk_sdf_buffer_instances_create(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count
)
{
    /* header buffer */
    buffer->total_header_buffer_size = nuvk_buffer_pad_uniform_buffer_size(context, sizeof(uint32_t) * type_count);

    nuvk_buffer_info_t header_info;
    memset(&header_info, 0, sizeof(nuvk_buffer_info_t));
    header_info.size         = buffer->total_header_buffer_size * render_context->max_inflight_frame_count;
    header_info.buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    header_info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    if (nuvk_buffer_create(&buffer->header_buffer, memory_manager, &header_info) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create instance header buffer.\n");
        return NU_FAILURE;
    }

    if (nuvk_buffer_map(&buffer->header_buffer, memory_manager) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to map instance header buffer.\n");
        return NU_FAILURE;
    }

    for (uint32_t frame = 0; frame < render_context->max_inflight_frame_count; frame++) {
        for (uint32_t type = 0; type < type_count; type++) {
            nuvk_sdf_buffer_instances_write_instance_count(buffer, frame, type, 0);
        }
    }
 
    /* instances buffer */

    buffer->type_arrays_offsets    = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);
    buffer->type_arrays_item_sizes = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);

    buffer->total_type_arrays_buffer_size = 0;
    for (uint32_t i = 0; i < type_count; i++) {
        buffer->type_arrays_item_sizes[i]      = (sizeof(nu_mat4f_t) + types[i].data_size); /* 16 bytes alignment */
        buffer->type_arrays_offsets[i]         = buffer->total_type_arrays_buffer_size;
        buffer->total_type_arrays_buffer_size += buffer->type_arrays_item_sizes[i] * NUVK_SDF_INSTANCE_PER_TYPE_MAX_COUNT;
    }

    buffer->total_type_arrays_buffer_size = nuvk_buffer_pad_uniform_buffer_size(context, buffer->total_type_arrays_buffer_size);

    nuvk_buffer_info_t type_arrays_info;
    memset(&type_arrays_info, 0, sizeof(nuvk_buffer_info_t));
    type_arrays_info.size         = buffer->total_type_arrays_buffer_size * render_context->max_inflight_frame_count;
    type_arrays_info.buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    type_arrays_info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    if (nuvk_buffer_create(&buffer->type_arrays_buffer, memory_manager, &type_arrays_info) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create instance type arrays buffer.\n");
        return NU_FAILURE;
    }

    if (nuvk_buffer_map(&buffer->type_arrays_buffer, memory_manager) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to map instance type arrays buffer.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_instances_destroy(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_buffer_unmap(&buffer->header_buffer, memory_manager);
    nuvk_buffer_destroy(&buffer->header_buffer, memory_manager);
    nuvk_buffer_unmap(&buffer->type_arrays_buffer, memory_manager);
    nuvk_buffer_destroy(&buffer->type_arrays_buffer, memory_manager);
    nu_free(buffer->type_arrays_offsets);
    nu_free(buffer->type_arrays_item_sizes);

    return NU_SUCCESS;
}