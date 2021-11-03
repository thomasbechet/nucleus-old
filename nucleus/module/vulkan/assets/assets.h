#ifndef NUVK_ASSETS_H
#define NUVK_ASSETS_H

#include <nucleus/module/vulkan/assets/sdf.h>
#include <nucleus/module/vulkan/assets/material.h>
#include <nucleus/module/vulkan/buffer/instances.h>
#include <nucleus/module/vulkan/buffer/materials.h>

typedef struct {
    nu_indexed_array_t sdfs;
    nu_indexed_array_t materials;
    nu_array_t material_free_buffer_indices;
} nuvk_assets_t;

nu_result_t nuvk_assets_initialize(nuvk_assets_t *assets);
nu_result_t nuvk_assets_terminate(nuvk_assets_t *assets);

nu_result_t nuvk_assets_sdf_create(
    nuvk_assets_t *assets,
    nuvk_buffer_instances_t *buffer,
    const nuvk_sdf_info_t *info,
    nuvk_sdf_t *handle
);
nu_result_t nuvk_assets_material_create(
    nuvk_assets_t *assets,
    nuvk_buffer_materials_t *buffer,
    uint32_t active_inflight_frame_index,
    const nuvk_material_info_t *info,
    nuvk_material_t *handle
);

#endif