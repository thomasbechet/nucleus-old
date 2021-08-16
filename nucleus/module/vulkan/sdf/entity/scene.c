#include <nucleus/module/vulkan/sdf/entity/scene.h>

nu_result_t nuvk_sdf_scene_initialize(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count,
    nuvk_sdf_instance_type_t *type_handles
)
{
    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene)
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