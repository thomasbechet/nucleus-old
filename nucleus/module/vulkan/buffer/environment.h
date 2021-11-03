#ifndef NUVK_BUFFER_ENVIRONMENT_H
#define NUVK_BUFFER_ENVIRONMENT_H

#include <nucleus/module/vulkan/core/dynamic_range_buffer.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/scene/camera.h>

typedef struct {
    uint32_t uniform_buffer_range;
    nuvk_dynamic_range_buffer_t dynamic_range_buffer;
} nuvk_buffer_environment_t;

nu_result_t nuvk_buffer_environment_create(
    nuvk_buffer_environment_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_buffer_environment_destroy(
    nuvk_buffer_environment_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_buffer_environment_write_camera(
    nuvk_buffer_environment_t *buffer,
    const nuvk_camera_t *camera,
    uint32_t active_inflight_frame_index
);

#endif