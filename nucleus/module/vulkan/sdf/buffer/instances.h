#ifndef NUVK_SDF_BUFFER_INSTANCE_H
#define NUVK_SDF_BUFFER_INSTANCE_H

#include <nucleus/module/vulkan/core/buffer.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/entity/interface.h>

typedef struct {
    /* buffer 0 */
    nuvk_buffer_t header_buffer;
    uint32_t total_header_buffer_size;
    /* buffer 1 */
    nuvk_buffer_t type_arrays_buffer;
    uint32_t total_type_arrays_buffer_size;
    uint32_t *type_arrays_offsets;
    uint32_t *type_arrays_item_sizes;
} nuvk_sdf_buffer_instances_t;

nu_result_t nuvk_sdf_buffer_instances_create(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count
);
nu_result_t nuvk_sdf_buffer_instances_destroy(
    nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

#endif