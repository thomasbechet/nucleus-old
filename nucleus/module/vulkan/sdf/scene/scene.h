#ifndef NUVK_SDF_SCENE_H
#define NUVK_SDF_SCENE_H

#include <nucleus/module/vulkan/sdf/scene/interface.h>
#include <nucleus/module/vulkan/sdf/scene/camera.h>
#include <nucleus/module/vulkan/sdf/buffer/environment.h>
#include <nucleus/module/vulkan/sdf/buffer/instances.h>

typedef struct {
    nu_aabb_t aabb;
    nu_transform_t transform;
    nu_mat3f_t inv_rotation;
    nu_vec3f_t translation;
    nu_vec3f_t scale;
    uint32_t index_position;
} nuvk_sdf_instance_data_t;

typedef struct {
    nuvk_sdf_instance_type_info_t info;

    nuvk_sdf_instance_data_t *instances;
    void *data;
    uint32_t instance_count;
    nu_array_t free_instance_indices;

    uint32_t *indices;
    uint32_t index_count;

    uint32_t *index_updates;
} nuvk_sdf_instance_type_data_t;

typedef struct {
    uint32_t type_index;
    uint32_t instance_index;
} nuvk_sdf_instance_handle_data_t;

typedef struct {
    nuvk_sdf_camera_t camera;
    nuvk_sdf_instance_type_data_t types[NUVK_SDF_MAX_INSTANCE_TYPE_COUNT];
    uint32_t type_count;
    nu_indexed_array_t instance_handles;
} nuvk_sdf_scene_t;

nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene);
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene);
nu_result_t nuvk_sdf_scene_update_buffers(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_buffer_environment_t *environment_buffer,
    nuvk_sdf_buffer_instances_t *instances_buffer
);

nu_result_t nuvk_sdf_scene_register_instance_type(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_type_info_t *info,
    nuvk_sdf_instance_type_t *handle
);
nu_result_t nuvk_sdf_scene_create_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_instance_info_t *info,
    nuvk_sdf_instance_t *handle
);
nu_result_t nuvk_sdf_scene_destroy_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_instance_t handle
);
nu_result_t nuvk_sdf_scene_update_instance_transform(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_instance_t handle,
    const nu_transform_t *transform
);
nu_result_t nuvk_sdf_scene_update_instance_data(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_instance_t handle,
    const void *data
);

#endif