#include "renderer.h"

#include "interface.h"
#include "../renderer.h"

static const uint32_t interface_count = 1;
static const char *interfaces[] = {
    NU_RENDERER_INTERFACE_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->type = NU_MODULE_TYPE_RENDERER;
    info->id = NUVK_MODULE_ID;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_interface_loader(nu_renderer_interface_t *interface)
{
    interface->initialize = nu_renderer_initialize;
    interface->terminate  = nu_renderer_terminate;
    interface->render     = nu_renderer_render;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_initialize(void)
{
    return nuvk_renderer_initialize();
}
nu_result_t nu_renderer_terminate(void)
{
    return nuvk_renderer_terminate();
}
nu_result_t nu_renderer_render(void)
{
    return nuvk_renderer_render();
}