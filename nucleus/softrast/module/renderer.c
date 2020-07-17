#include "renderer.h"

#include "../softrast.h"
#include "../asset/mesh.h"
#include "../scene/scene.h"

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_RENDERER_INTERFACE_NAME,
    NUSR_RENDERER_INTERFACE_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->id = NUSR_MODULE_ID;
    info->type = NU_MODULE_TYPE_RENDERER;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_interface_loader(nu_renderer_interface_t *interface)
{
    interface->initialize = nusr_initialize;
    interface->terminate  = nusr_terminate;
    interface->render     = nusr_render;

    return NU_SUCCESS;
}
nu_result_t nusr_renderer_interface_loader(nusr_renderer_interface_t *interface)
{
    interface->mesh_create  = nusr_mesh_create;
    interface->mesh_destroy = nusr_mesh_destroy;
    
    interface->camera_set_fov    = nusr_scene_camera_set_fov;
    interface->camera_set_eye    = nusr_scene_camera_set_eye;
    interface->camera_set_center = nusr_scene_camera_set_center;

    interface->staticmesh_create        = nusr_scene_staticmesh_create;
    interface->staticmesh_destroy       = nusr_scene_staticmesh_destroy;
    interface->staticmesh_set_transform = nusr_scene_staticmesh_set_transform;

    return NU_SUCCESS;
}