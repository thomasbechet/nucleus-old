#include "renderer.h"

#include "../softrast.h"

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
    interface->terminate = nusr_terminate;
    interface->render = nusr_render;

    return NU_SUCCESS;
}
nu_result_t nusr_renderer_interface_loader(nusr_renderer_interface_t *interface)
{
    return NU_SUCCESS;
}