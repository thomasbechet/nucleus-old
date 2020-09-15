#include "module.hpp"

#include "interface.h"
#include "../console/console.hpp"
#include "../command/command.hpp"

static const uint32_t plugin_count = 2;
static const char *plugins[] = {
    NU_UTILITY_PLUGIN_CONSOLE_NAME,
    NU_UTILITY_PLUGIN_COMMAND_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->flags = NU_MODULE_FLAG_TYPE_PLUGIN;
    info->id = NU_UTILITY_MODULE_ID;
    info->plugin_count = plugin_count;
    info->plugins = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_plugin_get_interface(nu_plugin_interface_t *interface, const char *name)
{
    if (NU_MATCH(name, NU_UTILITY_PLUGIN_CONSOLE_NAME)) {
        interface->initialize = nu_utility_plugin_console_initialize;
        interface->terminate  = nu_utility_plugin_console_terminate;
        interface->update     = nu_utility_plugin_console_update;
    } else if (NU_MATCH(name, NU_UTILITY_PLUGIN_COMMAND_NAME)) {
        interface->initialize = nu_utility_plugin_command_initialize;
        interface->terminate  = nu_utility_plugin_command_terminate;
        interface->update     = nu_utility_plugin_command_update;
    }

    return NU_SUCCESS;
}