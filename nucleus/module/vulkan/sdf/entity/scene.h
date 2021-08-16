#ifndef NUVK_SDF_SCENE_H
#define NUVK_SDF_SCENE_H

#include <nucleus/module/vulkan/sdf/entity/interface.h>

#define NUVK_SDF_SCENE_MAX_INSTANCE_PER_TYPE_COUNT 64
#define NUVK_SDF_SCENE_MAX_INSTANCE_TYPE_COUNT     32

/* [HEADER][DATA][HEADER][DATA]... */
typedef struct {
    nu_aabb_t aabb;
    nu_transform_t transform;
    nu_mat4f_t inv_matrix;
    uint32_t gpu_instance_index;
} nuvk_sdf_instance_header_t;

typedef struct {
    nuvk_sdf_instance_type_info_t info;
    uint32_t instance_header_data_size;
    uint32_t instance_count;
    void *instances;
} nuvk_sdf_instance_type_data_t;

typedef struct {
    nu_indexed_array_t types;
    nu_indexed_array_t instances;

    // VOXELS => use instanced rendering
} nuvk_sdf_scene_t;

nu_result_t nuvk_sdf_scene_initialize(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count,
    nuvk_sdf_instance_type_t *type_handles
);
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene);

nu_result_t nuvk_sdf_scene_create_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_info_t *info,
    nuvk_sdf_instance_t *handle
);

#endif