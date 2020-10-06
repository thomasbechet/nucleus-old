#include "module.hpp"

#include "interface.h"
#include "../console/console.hpp"
#include "../command/command.hpp"
#include "../loader/loader.hpp"

static const uint32_t plugin_count = 3;
static const char *plugins[] = {
    NUUTILS_CONSOLE_PLUGIN_NAME,
    NUUTILS_COMMAND_PLUGIN_NAME,
    NUUTILS_LOADER_PLUGIN_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name         = NUUTILS_MODULE_NAME;
    info->id           = NUUTILS_MODULE_ID;
    info->flags        = NU_MODULE_FLAG_TYPE_PLUGIN;
    info->plugin_count = plugin_count;
    info->plugins      = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_plugin_get_interface(nu_plugin_interface_t *interface, const char *name)
{
    if (NU_MATCH(name, NUUTILS_CONSOLE_PLUGIN_NAME)) {
        interface->initialize = nuutils_console_plugin_initialize;
        interface->terminate  = nuutils_console_plugin_terminate;
        interface->update     = nuutils_console_plugin_update;
    } else if (NU_MATCH(name, NUUTILS_COMMAND_PLUGIN_NAME)) {
        interface->initialize = nuutils_command_plugin_initialize;
        interface->terminate  = nuutils_command_plugin_terminate;
    } else if (NU_MATCH(name, NUUTILS_LOADER_PLUGIN_NAME)) {
        /* no interface */
    }

    return NU_SUCCESS;
}