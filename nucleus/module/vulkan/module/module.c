#include <nucleus/module/vulkan/module/module.h>

#include <nucleus/module/vulkan/renderer.h>

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_RENDERER_INTERFACE_NAME,
    NUVK_SDF_INTERFACE_NAME
};

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name            = NUVK_MODULE_NAME;
    info->id              = NUVK_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_RENDERER_INTERFACE_NAME)) {
        nu_renderer_interface_t *i = (nu_renderer_interface_t*)interface;

        i->initialize = nuvk_renderer_initialize;
        i->terminate  = nuvk_renderer_terminate;
        i->render     = nuvk_renderer_render;

        i->camera_set_fov  = nuvk_renderer_camera_set_fov;
        i->camera_set_view = nuvk_renderer_camera_set_view;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUVK_SDF_INTERFACE_NAME)) {
        nuvk_sdf_interface_t *i = (nuvk_sdf_interface_t*)interface;

        i->instance_type_register    = nuvk_sdf_instance_type_register;
        i->instance_create           = nuvk_sdf_instance_create;
        i->instance_destroy          = nuvk_sdf_instance_destroy;
        i->instance_update_transform = nuvk_sdf_instance_update_transform;
        i->instance_update_data      = nuvk_sdf_instance_update_data;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}