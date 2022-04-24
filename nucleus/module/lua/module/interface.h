/* Generated file: DO NOT EDIT ! */
#ifndef NULUA_MODULE_INTERFACE_H
#define NULUA_MODULE_INTERFACE_H

#include <nucleus/module/lua/module/definition.h>

/* loader */
#ifdef NULUA_LOADER_IMPLEMENTATION
    nulua_plugin_load_pfn_t nulua_plugin_load = NULL;
    nu_result_t nulua_plugin_interface_load(nu_module_t module)
    {
        nulua_plugin_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NULUA_PLUGIN_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nulua_plugin_load = interface.load;
        }
        return result;
    }
    nu_result_t nulua_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nulua_plugin_interface_load(module);
        return result;
    }
#else
    extern nulua_plugin_load_pfn_t nulua_plugin_load;
    nu_result_t nulua_plugin_interface_load(nu_module_t module);
    nu_result_t nulua_interface_load_all(nu_module_t module);
#endif

#endif