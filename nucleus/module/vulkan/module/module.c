/* Generated file: DO NOT EDIT ! */
#include <nucleus/module/vulkan/module/module.h>

#include <nucleus/module/vulkan/module/definition.h>
#include <nucleus/module/vulkan/renderer.h>

static const uint32_t interface_count = 3;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME, 
    NU_RENDERER_INTERFACE_NAME, 
    NUVK_RENDERER_INTERFACE_NAME
};

static const uint32_t plugin_count = 0;
static const char *plugins[] = {
};

static nu_result_t plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{

    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name = NUVK_MODULE_NAME;
    info->id = NUVK_MODULE_ID;
    info->flags = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_PLUGIN_INTERFACE_NAME)) {
        nu_plugin_interface_t *i = (nu_plugin_interface_t*)interface;

        i->get_callbacks = plugin_get_callbacks;
        i->get_list = plugin_get_list;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NU_RENDERER_INTERFACE_NAME)) {
        nu_renderer_interface_t *i = (nu_renderer_interface_t*)interface;
        
        i->initialize = nuvk_renderer_initialize;
        i->terminate = nuvk_renderer_terminate;
        i->start = nuvk_renderer_start;
        i->stop = nuvk_renderer_stop;
        i->render = nuvk_renderer_render;
        i->camera_create = nuvk_renderer_camera_create;
        i->camera_destroy = nuvk_renderer_camera_destroy;
        i->camera_set_fov = nuvk_renderer_camera_set_fov;
        i->camera_set_view = nuvk_renderer_camera_set_view;
        i->viewport_set_size = nuvk_renderer_viewport_set_size;
        i->viewport_get_size = nuvk_renderer_viewport_get_size;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUVK_RENDERER_INTERFACE_NAME)) {
        nuvk_renderer_interface_t *i = (nuvk_renderer_interface_t*)interface;
        
        i->sdf_create = nuvk_renderer_sdf_create;
        i->sdf_destroy = nuvk_renderer_sdf_destroy;
        i->sdf_get_primitive = nuvk_renderer_sdf_get_primitive;
        i->material_create = nuvk_renderer_material_create;
        i->material_destroy = nuvk_renderer_material_destroy;
        i->sdf_instance_create = nuvk_renderer_sdf_instance_create;
        i->sdf_instance_destroy = nuvk_renderer_sdf_instance_destroy;
        i->sdf_instance_update_transform = nuvk_renderer_sdf_instance_update_transform;
        i->sdf_instance_update_data = nuvk_renderer_sdf_instance_update_data;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}