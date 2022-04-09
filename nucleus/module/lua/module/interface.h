/* Generated file: DO NOT EDIT ! */
#ifndef NULUA_MODULE_INTERFACE_H
#define NULUA_MODULE_INTERFACE_H

#include <nucleus/module/lua/module/definition.h>

/* functions definition */
typedef nu_result_t (*__nulua_plugin_load_pfn_t)(const char* path, nulua_plugin_t* handle);

/* loader */
#ifdef NULUA_LOADER_IMPLEMENTATION
    __nulua_plugin_load_pfn_t __nulua_plugin_load = NULL;
    nu_result_t nulua_plugin_interface_load(nu_module_t module)
    {
        nulua_plugin_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NULUA_PLUGIN_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nulua_plugin_load = interface.load;
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
    extern __nulua_plugin_load_pfn_t __nulua_plugin_load;
    nu_result_t nulua_plugin_interface_load(nu_module_t module);
    nu_result_t nulua_interface_load_all(nu_module_t module);
#endif
#define nulua_plugin_load NU_IDENTITY(__nulua_plugin_load)

#endif