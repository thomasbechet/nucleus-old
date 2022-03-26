/* Generated file: DO NOT EDIT ! */
#include <nucleus/module/lua/module/module.h>

#include <nucleus/module/lua/module/definition.h>
#include <nucleus/module/lua/module/implementation.h>

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME, 
    NULUA_PLUGIN_INTERFACE_NAME
};

static const uint32_t plugin_count = 1;
static const char *plugins[] = {
    NULUA_MANAGER_PLUGIN_NAME
};

static nu_result_t plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{
    if (NU_MATCH(name, NULUA_MANAGER_PLUGIN_NAME)) {
        callbacks->initialize = nulua_manager_plugin_initialize_impl;
        callbacks->terminate = nulua_manager_plugin_terminate_impl;
        callbacks->update = nulua_manager_plugin_update_impl;
        return NU_SUCCESS;
    }

    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name = NULUA_MODULE_NAME;
    info->id = NULUA_MODULE_ID;
    info->flags = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_PLUGIN_INTERFACE_NAME)) {
        nu_plugin_interface_t *i = (nu_plugin_interface_t*)interface;

        i->get_callbacks = plugin_get_callbacks;
        i->get_list = plugin_get_list;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NULUA_PLUGIN_INTERFACE_NAME)) {
        nulua_plugin_interface_t *i = (nulua_plugin_interface_t*)interface;
        
        i->load = nulua_plugin_load_impl;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}