#include <nucleus/module/vulkan/module/module.h>

#include <nucleus/module/vulkan/renderer.h>

static const uint32_t interface_count = 1;
static const char *interfaces[] = {
    NU_RENDERER_INTERFACE_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name            = NUVK_MODULE_NAME;
    info->id              = NUVK_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;
    info->plugin_count    = 0;
    info->plugins         = NULL;

    return NU_SUCCESS;
}
nu_result_t nu_module_get_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_RENDERER_INTERFACE_NAME)) {
        nu_renderer_interface_t *i = (nu_renderer_interface_t*)interface;

        i->initialize = nuvk_renderer_initialize;
        i->terminate  = nuvk_renderer_terminate;
        i->render     = nuvk_renderer_render;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}