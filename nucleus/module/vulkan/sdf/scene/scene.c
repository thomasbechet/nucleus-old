#include <nucleus/module/vulkan/sdf/scene/scene.h>

typedef struct {
    nu_aabb_t aabb;
    nu_transform_t transform;
    nu_mat4f_t inv_matrix;
    uint32_t gpu_instance_index;
    uint32_t data_index;
} nuvk_sdf_instance_data_t;

typedef struct {
    nuvk_sdf_instance_type_info_t info;

    nuvk_sdf_instance_data_t *instances;
    uint32_t instance_count;
    nu_array_t free_instance_indices;

    void *data;
    nu_array_t free_data_indices;

    uint32_t *indices;
    uint32_t index_count;

    uint32_t *instance_updates;
} nuvk_sdf_instance_type_data_t;

typedef struct {
    uint32_t type_index;
    uint32_t instance_index;
} nuvk_sdf_instance_handle_data_t;

nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene)
{
    nu_array_allocate_capacity(sizeof(nuvk_sdf_instance_type_data_t), NUVK_SDF_MAX_INSTANCE_TYPE_COUNT, &scene->instance_types);
    nu_indexed_array_allocate(sizeof(nuvk_sdf_instance_handle_data_t), &scene->instance_handles);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene)
{
    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_update(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_sdf_camera_t *camera
)
{
    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_scene_register_instance_type(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_buffer_instances_t *buffer,
    const nuvk_sdf_instance_type_info_t *info
)
{
    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_create_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_info_t *info,
    nuvk_sdf_instance_t *handle
)
{


    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_destroy_instance(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_instance_t handle
)
{


    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_update_instance_transform(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_instance_t handle,
    const nu_transform_t *transform
)
{


    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_update_instance_data(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_instance_t handle,
    const void *data
)
{


    return NU_SUCCESS;
}