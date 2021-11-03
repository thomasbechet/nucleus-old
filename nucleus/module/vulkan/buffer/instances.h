#ifndef NUVK_BUFFER_INSTANCES_H
#define NUVK_BUFFER_INSTANCES_H

#include <nucleus/module/vulkan/core/dynamic_range_buffer.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/scene/interface.h>
#include <nucleus/module/vulkan/assets/sdf.h>

typedef struct {
    /* index buffer */
    nuvk_dynamic_range_buffer_t index_buffer;
    uint32_t index_uniform_buffer_range;
    uint32_t next_index_offset;
    uint32_t *index_offsets;
    /* instance buffer */
    nuvk_dynamic_range_buffer_t instance_buffer;
    uint32_t instance_uniform_buffer_range;
    uint32_t next_instance_offset;
    uint32_t *instance_offsets;
    uint32_t *instance_sizes;
    /* material buffer */
    nuvk_dynamic_range_buffer_t material_buffer;
    uint32_t material_uniform_buffer_range;
} nuvk_buffer_instances_t;

nu_result_t nuvk_buffer_instances_create(
    nuvk_buffer_instances_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_buffer_instances_destroy(
    nuvk_buffer_instances_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_buffer_instances_configure_sdfs(
    nuvk_buffer_instances_t *buffer,
    nuvk_sdf_data_t **sdfs,
    uint32_t sdf_count
);
nu_result_t nuvk_buffer_instances_write_indices(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t *indices,
    uint32_t index_count
);
nu_result_t nuvk_buffer_instances_write_instance_transform(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t instance_index,
    const nu_mat3f_t inv_rotation,
    const nu_vec4f_t translation_scale
);
nu_result_t nuvk_buffer_instances_write_instance_data(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t instance_index,
    const void *data
);
nu_result_t nuvk_buffer_instances_write_instance_material(
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t sdf_index,
    uint32_t instance_index,
    uint32_t material_index
);

#endif