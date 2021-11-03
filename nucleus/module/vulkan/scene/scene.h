#ifndef NUVK_SCENE_H
#define NUVK_SCENE_H

#include <nucleus/module/vulkan/scene/interface.h>
#include <nucleus/module/vulkan/scene/camera.h>
#include <nucleus/module/vulkan/buffer/environment.h>
#include <nucleus/module/vulkan/buffer/instances.h>
#include <nucleus/module/vulkan/buffer/materials.h>

#include <nucleus/module/vulkan/assets/assets.h>

typedef struct {
    nu_aabb_t aabb;
    nuvk_sdf_transform_t transform;
    nu_mat3f_t inv_rotation;
    nu_vec4f_t translation_scale;
    nuvk_material_data_t *material;
    uint32_t buffer_index;
    void *data;
} nuvk_sdf_instance_data_t;

typedef struct {
    nu_indexed_array_t instances;
    nu_array_t instance_free_buffer_indices;
    nuvk_sdf_data_t *sdf_data;
    uint32_t *indices;
} nuvk_sdf_pool_t;

typedef struct {
    uint32_t pool_id;
    uint32_t instance_id;
} nuvk_sdf_instance_handle_data_t;

typedef struct {
    nuvk_camera_t camera;
    nu_indexed_array_t instance_handles;
    nu_indexed_array_t sdf_pools;
} nuvk_scene_t;

nu_result_t nuvk_scene_initialize(nuvk_scene_t *scene);
nu_result_t nuvk_scene_terminate(nuvk_scene_t *scene);

nu_result_t nuvk_scene_sdf_pool_create(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    nuvk_sdf_data_t *sdf_data
);

nu_result_t nuvk_scene_sdf_instance_create(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    const nuvk_sdf_instance_info_t *info, 
    nuvk_sdf_instance_t *handle
);
nu_result_t nuvk_scene_sdf_instance_destroy(
    nuvk_scene_t *scene,
    nuvk_sdf_instance_t handle
);
nu_result_t nuvk_scene_sdf_instance_update_transform(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    nuvk_sdf_instance_t handle,
    const nuvk_sdf_transform_t *transform
);
nu_result_t nuvk_scene_sdf_instance_update_data(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    nuvk_sdf_instance_t handle,
    const void *data
);

// nu_result_t nuvk_sdf_scene_register_instance_type(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_sdf_instance_type_info_t *info,
//     nuvk_sdf_instance_type_t *handle
// );
// nu_result_t nuvk_sdf_scene_create_instance(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     const nuvk_sdf_instance_info_t *info,
//     nuvk_sdf_instance_t *handle
// );
// nu_result_t nuvk_sdf_scene_destroy_instance(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle
// );
// nu_result_t nuvk_sdf_scene_update_instance_transform(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle,
//     const nuvk_sdf_transform_t *transform
// );
// nu_result_t nuvk_sdf_scene_update_instance_data(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle,
//     const void *data
// );
// nu_result_t nuvk_sdf_scene_update_instance_material(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle,
//     nuvk_sdf_material_t material
// );

#endif