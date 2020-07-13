#include "renderer.h"

#include "../rasterizer.h"

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_RENDERER_INTERFACE_NAME,
    NURZ_RENDERER_INTERFACE_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->id = NURZ_MODULE_ID;
    info->type = NU_MODULE_TYPE_RENDERER;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_interface_loader(nu_renderer_interface_t *interface)
{
    interface->initialize = nurz_initialize;
    interface->terminate = nurz_terminate;
    interface->render = nurz_render;

    return NU_SUCCESS;
}
nu_result_t nurz_renderer_interface_loader(nurz_renderer_interface_t *interface)
{
    return NU_SUCCESS;
}