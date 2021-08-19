#ifndef NUVK_SDF_SCENE_H
#define NUVK_SDF_SCENE_H

#include <nucleus/module/vulkan/sdf/scene/interface.h>
#include <nucleus/module/vulkan/sdf/scene/camera.h>
#include <nucleus/module/vulkan/sdf/buffer/instances.h>

typedef struct {
    nu_array_t instance_types;
    nu_indexed_array_t instance_handles;
} nuvk_sdf_scene_t;

/* Note: use voxel rendering with instanced rendering */

nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene);
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene);
nu_result_t nuvk_sdf_scene_update(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_sdf_camera_t *camera
);

nu_result_t nuvk_sdf_scene_register_instance_type(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_sdf_instance_type_info_t *info
);
nu_result_t nuvk_sdf_scene_create_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_info_t *info,
    nuvk_sdf_instance_t *handle
);
nu_result_t nuvk_sdf_scene_destroy_instance(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_instance_t handle
);
nu_result_t nuvk_sdf_scene_update_instance_transform(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_instance_t handle,
    const nu_transform_t *transform
);
nu_result_t nuvk_sdf_scene_update_instance_data(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_instance_t handle,
    const void *data
);

#endif