#ifndef NUVK_SDF_SCENE_H
#define NUVK_SDF_SCENE_H

#include <nucleus/module/vulkan/sdf/entity/interface.h>

#define NUVK_SDF_SCENE_MAX_INSTANCE_PER_TYPE_COUNT 64
#define NUVK_SDF_SCENE_MAX_INSTANCE_TYPE_COUNT     32

typedef struct {
    nu_aabb_t aabb;
    nu_transform_t transform;
    uint32_t data_index;
} nuvk_sdf_instance_data_t;

typedef struct {
    nuvk_sdf_instance_sdf_pfn_t c_sdf_code;
    nuvk_sdf_instance_aabb_pfn_t c_aabb_code;
    nu_indexed_array_t instances;
    nu_array_t data;
} nuvk_sdf_instance_type_data_t;

typedef struct {
    nu_indexed_array_t types;
} nuvk_sdf_scene_t;

nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene);
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene);
nu_result_t nuvk_sdf_scene_register_instance_type(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_type_info_t *info,
    nuvk_sdf_instance_type_t *handle
);
nu_result_t nuvk_sdf_scene_create_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_info_t *info,
    nuvk_sdf_instance_t *handle
);

#endif