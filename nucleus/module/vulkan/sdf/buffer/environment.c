#include <nucleus/module/vulkan/sdf/buffer/environment.h>

nu_result_t nuvk_sdf_buffer_environment_create(
    nuvk_sdf_buffer_environment_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
)
{
    buffer->uniform_buffer_range = nuvk_buffer_pad_uniform_buffer_size(context, sizeof(nuvk_sdf_buffer_environment_data_t));

    nuvk_buffer_info_t info;
    info.size         = buffer->uniform_buffer_range * render_context->max_inflight_frame_count;
    info.buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    if (nuvk_buffer_create(&buffer->buffer, memory_manager, &info) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create environment buffer.\n");
        return NU_FAILURE;
    }

    if (nuvk_buffer_map(&buffer->buffer, memory_manager) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to map environment buffer.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_environment_destroy(
    nuvk_sdf_buffer_environment_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_buffer_unmap(&buffer->buffer, memory_manager);
    nuvk_buffer_destroy(&buffer->buffer, memory_manager);

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_buffer_environment_write_camera(
    nuvk_sdf_buffer_environment_t *buffer,
    const nuvk_sdf_camera_t *camera,
    uint32_t active_inflight_frame_index
)
{
    char *pdata = (char*)buffer->buffer.map + buffer->uniform_buffer_range * active_inflight_frame_index;
    nuvk_sdf_buffer_environment_data_t *data = (nuvk_sdf_buffer_environment_data_t*)pdata;
    nu_mat4f_copy(camera->vp_matrix, data->vp_matrix);
    nu_vec3f_copy(camera->eye, data->eye);

    return NU_SUCCESS;
}