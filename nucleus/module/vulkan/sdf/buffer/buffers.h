#ifndef NUVK_SDF_BUFFERS_H
#define NUVK_SDF_BUFFERS_H

#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/buffer/environment.h>
#include <nucleus/module/vulkan/sdf/buffer/instances.h>
#include <nucleus/module/vulkan/sdf/entity/primitives.h>

typedef struct {
    nuvk_sdf_buffer_environment_t environment;
    nuvk_sdf_buffer_instances_t instances;
} nuvk_sdf_buffers_t;

nu_result_t nuvk_sdf_buffers_initialize(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_primitives_t *primitives
);
nu_result_t nuvk_sdf_buffers_terminate(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_memory_manager_t *memory_manager
);

#endif