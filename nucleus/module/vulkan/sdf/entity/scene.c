#include <nucleus/module/vulkan/sdf/entity/scene.h>

nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene)
{
    nu_indexed_array_allocate(sizeof(nuvk_sdf_instance_type_data_t), &scene->types);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene)
{
    uint32_t type_count = nu_indexed_array_get_size(scene->types);
    nuvk_sdf_instance_type_data_t *types = (nuvk_sdf_instance_type_data_t*)nu_indexed_array_get_data(scene->types);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_indexed_array_free(types[i].instances);
        nu_array_free(types[i].data);
    }
    nu_indexed_array_free(scene->types);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_register_instance_type(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_type_info_t *info,
    nuvk_sdf_instance_type_t *handle
)
{
    // instances ...
    // -> transform
    // -> type_id
    // -> data_index

    // sphere_data ...
    // cube_data ...

    uint32_t id;
    nu_indexed_array_add(scene->types, NULL, &id);
    nuvk_sdf_instance_type_data_t *type = (nuvk_sdf_instance_type_data_t*)nu_indexed_array_get(scene->types, id);

    type->c_sdf_code  = info->c_sdf_code;
    type->c_aabb_code = info->c_aabb_code;
    nu_indexed_array_allocate(sizeof(nuvk_sdf_instance_data_t), &type->instances);
    nu_array_allocate(info->data_size, &type->data);

    NU_HANDLE_SET_ID(*handle, id);

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