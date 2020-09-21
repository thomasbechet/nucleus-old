#include "module.h"

#include "interface.h"
#include "../renderer.h"

static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name         = NUVK_MODULE_NAME;
    info->id           = NUVK_MODULE_ID;
    info->flags        = NU_MODULE_FLAG_TYPE_RENDERER;
    info->plugin_count = plugin_count;
    info->plugins      = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_renderer_interface_loader(nu_renderer_interface_t *interface)
{
    interface->initialize = nuvk_renderer_initialize;
    interface->terminate  = nuvk_renderer_terminate;
    interface->render     = nuvk_renderer_render;

    return NU_SUCCESS;
}