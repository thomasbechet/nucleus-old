#include <nucleus/module/vulkan/buffer/environment.h>

typedef struct {
    nu_mat4f_t vp_matrix;
    nu_vec3f_t eye;
    float pixel_radius_factor;
} nuvk_buffer_environment_data_t;

nu_result_t nuvk_buffer_environment_create(
    nuvk_buffer_environment_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
)
{
    buffer->uniform_buffer_range = nuvk_buffer_pad_uniform_buffer_size(context, sizeof(nuvk_buffer_environment_data_t));

    nu_result_t result = nuvk_dynamic_range_buffer_create(
        &buffer->dynamic_range_buffer, memory_manager, 
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        buffer->uniform_buffer_range,
        render_context->max_inflight_frame_count,
        32
    );
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create environment buffer.");

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_environment_destroy(
    nuvk_buffer_environment_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_dynamic_range_buffer_destroy(&buffer->dynamic_range_buffer, memory_manager);
    return NU_SUCCESS;
}

nu_result_t nuvk_buffer_environment_write_camera(
    nuvk_buffer_environment_t *buffer,
    const nuvk_camera_t *camera,
    uint32_t active_inflight_frame_index
)
{
    nuvk_buffer_environment_data_t buf;
    nu_mat4f_copy(camera->vp_matrix, buf.vp_matrix);
    nu_vec3f_copy(camera->eye, buf.eye);
    buf.pixel_radius_factor = ((1.0f / NU_MIN(camera->resolution[0], camera->resolution[1])) * 0.5f) / camera->focal_length;

    nuvk_dynamic_range_buffer_write(
        &buffer->dynamic_range_buffer, active_inflight_frame_index,
        0, sizeof(nuvk_buffer_environment_data_t), &buf);

    return NU_SUCCESS;
}