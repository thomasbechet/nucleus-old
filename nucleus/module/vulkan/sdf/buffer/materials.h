#ifndef NUVK_SDF_BUFFER_MATERIALS_H
#define NUVK_SDF_BUFFER_MATERIALS_H

#include <nucleus/module/vulkan/core/dynamic_range_buffer.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/scene/interface.h>

typedef struct {
    uint32_t uniform_buffer_range;
    nuvk_dynamic_range_buffer_t dynamic_range_buffer;
} nuvk_sdf_buffer_materials_t;

nu_result_t nuvk_sdf_buffer_materials_create(
    nuvk_sdf_buffer_materials_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_sdf_buffer_materials_destroy(
    nuvk_sdf_buffer_materials_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_sdf_buffer_materials_write_material(
    nuvk_sdf_buffer_materials_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t material_index,
    const nuvk_sdf_material_info_t *info
);

#endif