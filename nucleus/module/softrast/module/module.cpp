#include "module.hpp"

#include "../renderer.hpp"

using namespace nu::softrast;

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_RENDERER_INTERFACE_NAME,
    NUSR_RENDERER_INTERFACE_NAME
};
static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name            = NUSR_MODULE_NAME;
    info->id              = NUSR_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;
    info->plugin_count    = plugin_count;
    info->plugins         = plugins;

    return NU_SUCCESS;
}
nu_result_t nu_module_get_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_RENDERER_INTERFACE_NAME)) {
        nu_renderer_interface_t *i = (nu_renderer_interface_t*)interface;

        i->initialize = initialize;
        i->terminate  = terminate;
        i->start      = start;
        i->stop       = stop;
        i->render     = render;

        i->mesh_create  = mesh_create;
        i->mesh_destroy = mesh_destroy;

        i->texture_create  = texture_create;
        i->texture_destroy = texture_destroy;

        i->material_create  = material_create;
        i->material_destroy = material_destroy;

        i->font_create        = font_create;
        i->font_destroy       = font_destroy;
        i->font_get_text_size = font_get_text_size;

        i->camera_create     = NULL;
        i->camera_destroy    = NULL;
        i->camera_set_fov    = camera_set_fov;
        i->camera_set_view   = camera_set_view;
        i->camera_set_active = NULL;

        i->model_create        = model_create;
        i->model_destroy       = model_destroy;
        i->model_set_transform = model_set_transform;

        i->label_create       = label_create;
        i->label_destroy      = label_destroy;
        i->label_set_position = label_set_position;
        i->label_set_text     = label_set_text;

        i->rectangle_create   = rectangle_create;
        i->rectangle_destroy  = rectangle_destroy;
        i->rectangle_set_rect = rectangle_set_rect;

        i->viewport_set_size = viewport_set_size;
        i->viewport_get_size = viewport_get_size;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUSR_RENDERER_INTERFACE_NAME)) {
        return NU_SUCCESS;
    }

    return NU_FAILURE;
}