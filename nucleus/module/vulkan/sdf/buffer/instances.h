#ifndef NUVK_SDF_BUFFER_INSTANCE_H
#define NUVK_SDF_BUFFER_INSTANCE_H

#include <nucleus/module/vulkan/core/buffer.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/scene/interface.h>

typedef struct {
    /* index buffer */
    nuvk_buffer_t index_buffer;
    uint32_t index_uniform_buffer_range;
    uint32_t next_index_offset;
    uint32_t *index_offsets;
    /* instance buffer */
    nuvk_buffer_t instance_buffer;
    uint32_t instance_uniform_buffer_range;
    uint32_t next_instance_offset;
    uint32_t *instance_offsets;
    uint32_t *instance_sizes;
} nuvk_sdf_buffer_instances_t;

nu_result_t nuvk_sdf_buffer_instances_create(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_sdf_buffer_instances_destroy(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_sdf_buffer_instances_configure_instance_types(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count
);
nu_result_t nuvk_sdf_buffer_instances_write_index_count(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t count
);
nu_result_t nuvk_sdf_buffer_instances_write_index(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t index_position,
    uint32_t index
);
nu_result_t nuvk_sdf_buffer_instances_write_instance_transform(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t instance_index,
    const nu_mat3f_t inv_rotation,
    const nu_vec4f_t translation_scale
);
nu_result_t nuvk_sdf_buffer_instances_write_instance_data(
    nuvk_sdf_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t type_index,
    uint32_t instance_index,
    void *data
);

#endif