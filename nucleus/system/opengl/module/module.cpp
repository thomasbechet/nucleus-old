#include "module.hpp"

#include "interface.h"
#include "../renderer.hpp"

using namespace nugl;

static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name         = NUGL_MODULE_NAME;
    info->id           = NUGL_MODULE_ID;
    info->flags        = NU_MODULE_FLAG_TYPE_RENDERER;
    info->plugin_count = plugin_count;
    info->plugins      = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_get_interface(nu_renderer_interface_t *interface)
{
    interface->initialize = initialize;
    interface->terminate  = terminate;
    interface->render     = render;

    interface->mesh_create  = mesh_create;
    interface->mesh_destroy = mesh_destroy;

    interface->texture_create  = texture_create;
    interface->texture_destroy = texture_destroy;

    interface->material_create  = material_create;
    interface->material_destroy = material_destroy;

    interface->font_create        = font_create;
    interface->font_destroy       = font_destroy;
    interface->font_get_text_size = font_get_text_size;

    interface->camera_create     = NULL;
    interface->camera_destroy    = NULL;
    interface->camera_set_fov    = camera_set_fov;
    interface->camera_set_eye    = camera_set_eye;
    interface->camera_set_center = camera_set_center;
    interface->camera_set_active = NULL;

    interface->staticmesh_create        = staticmesh_create;
    interface->staticmesh_destroy       = staticmesh_destroy;
    interface->staticmesh_set_transform = staticmesh_set_transform;

    interface->label_create       = label_create;
    interface->label_destroy      = label_destroy;
    interface->label_set_position = label_set_position;
    interface->label_set_text     = label_set_text;

    interface->rectangle_create   = rectangle_create;
    interface->rectangle_destroy  = rectangle_destroy;
    interface->rectangle_set_rect = rectangle_set_rect;

    interface->viewport_set_size = viewport_set_size;
    interface->viewport_get_size = viewport_get_size;

    return NU_SUCCESS;
}