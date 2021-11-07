#include <nucleus/module/ecs/module/module.h>

#include <nucleus/module/ecs/module/interface.h>

static const uint32_t interface_count = 0;
static const char *interfaces[] = {};

static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

static nu_result_t nuecs_plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t nuecs_plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{
    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name            = NUECS_MODULE_NAME;
    info->id              = NUECS_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    return NU_FAILURE;
}