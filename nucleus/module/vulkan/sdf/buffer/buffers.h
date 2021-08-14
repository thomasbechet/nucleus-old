#ifndef NUVK_SDF_BUFFERS_H
#define NUVK_SDF_BUFFERS_H

#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/buffer/environment.h>

typedef struct {
    nuvk_sdf_buffer_environment_t environment;
} nuvk_sdf_buffers_t;

nu_result_t nuvk_sdf_buffers_initialize(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_sdf_buffers_terminate(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);

#endif