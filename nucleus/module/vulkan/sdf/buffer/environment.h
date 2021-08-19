#ifndef NUVK_SDF_BUFFER_ENVIRONMENT_H
#define NUVK_SDF_BUFFER_ENVIRONMENT_H

#include <nucleus/module/vulkan/core/buffer.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/scene/camera.h>

typedef struct {
    nu_mat4f_t vp_matrix;
    nu_vec3f_t eye;
} nuvk_sdf_buffer_environment_data_t;

typedef struct {
    uint32_t uniform_buffer_range;
    nuvk_buffer_t buffer;
} nuvk_sdf_buffer_environment_t;

nu_result_t nuvk_sdf_buffer_environment_create(
    nuvk_sdf_buffer_environment_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_sdf_buffer_environment_destroy(
    nuvk_sdf_buffer_environment_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_sdf_buffer_environment_write_camera(
    nuvk_sdf_buffer_environment_t *buffer,
    const nuvk_sdf_camera_t *camera,
    uint32_t active_inflight_frame_index
);

#endif