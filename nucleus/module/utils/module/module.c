#include <nucleus/module/utils/module/module.h>

#include <nucleus/module/utils/module/interface.h>
#include <nucleus/module/utils/console/console.h>
#include <nucleus/module/utils/command/command.h>
#include <nucleus/module/utils/loader/loader.h>

static const uint32_t interface_count = 3;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME,
    NUUTILS_LOADER_INTERFACE_NAME,
    NUUTILS_COMMAND_INTERFACE_NAME
};

static const uint32_t plugin_count = 3;
static const char *plugins[] = {
    NUUTILS_CONSOLE_PLUGIN_NAME,
    NUUTILS_COMMAND_PLUGIN_NAME
};

static nu_result_t nuutils_plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t nuutils_plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{
    if (NU_MATCH(name, NUUTILS_CONSOLE_PLUGIN_NAME)) {
        callbacks->initialize = nuutils_console_plugin_initialize;
        callbacks->terminate  = nuutils_console_plugin_terminate;
        callbacks->update     = nuutils_console_plugin_update;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUUTILS_COMMAND_PLUGIN_NAME)) {
        callbacks->initialize = nuutils_command_plugin_initialize;
        callbacks->terminate  = nuutils_command_plugin_terminate;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name            = NUUTILS_MODULE_NAME;
    info->id              = NUUTILS_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_PLUGIN_INTERFACE_NAME)) {
        nu_plugin_interface_t *i = (nu_plugin_interface_t*)interface;

        i->get_callbacks      = nuutils_plugin_get_callbacks;
        i->get_list           = nuutils_plugin_get_list;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUUTILS_LOADER_INTERFACE_NAME)) {
        nuutils_loader_interface_t *i = (nuutils_loader_interface_t*)interface;

        i->load_mesh_from_obj = nuutils_load_mesh_from_obj;
        i->load_texture       = nuutils_load_texture;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUUTILS_COMMAND_INTERFACE_NAME)) {
        nuutils_command_interface_t *i = (nuutils_command_interface_t*)interface;

        i->execute = nuutils_command_execute;
        i->get_event_id = nuutils_command_get_event_id;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}