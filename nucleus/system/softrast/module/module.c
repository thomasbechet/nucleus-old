#include "module.h"

#include "../softrast.h"
#include "../asset/mesh.h"
#include "../asset/texture.h"
#include "../asset/font.h"
#include "../scene/scene.h"
#include "../viewport/viewport.h"
#include "../gui/gui.h"

static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->id = NUSR_MODULE_ID;
    info->flags = NU_MODULE_FLAG_TYPE_RENDERER;
    info->plugin_count = plugin_count;
    info->plugins = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_get_interface(nu_renderer_interface_t *interface)
{
    interface->initialize = nusr_initialize;
    interface->terminate  = nusr_terminate;
    interface->render     = nusr_render;

    interface->mesh_create  = nusr_mesh_create;
    interface->mesh_destroy = nusr_mesh_destroy;

    interface->texture_create  = nusr_texture_create;
    interface->texture_destroy = nusr_texture_destroy;

    interface->font_create  = nusr_font_create;
    interface->font_destroy = nusr_font_destroy;

    interface->camera_create     = NULL;
    interface->camera_destroy    = NULL;
    interface->camera_set_fov    = nusr_scene_camera_set_fov;
    interface->camera_set_eye    = nusr_scene_camera_set_eye;
    interface->camera_set_center = nusr_scene_camera_set_center;
    interface->camera_set_active = NULL;

    interface->staticmesh_create        = nusr_scene_staticmesh_create;
    interface->staticmesh_destroy       = nusr_scene_staticmesh_destroy;
    interface->staticmesh_set_transform = nusr_scene_staticmesh_set_transform;

    interface->label_create       = nusr_gui_label_create;
    interface->label_destroy      = nusr_gui_label_destroy;
    interface->label_set_position = nusr_gui_label_set_position;
    interface->label_set_text     = nusr_gui_label_set_text;

    interface->viewport_get_size = nusr_viewport_get_size;

    return NU_SUCCESS;
}
nu_result_t nusr_renderer_get_interface(nusr_renderer_interface_t *interface)
{
    return NU_SUCCESS;
}