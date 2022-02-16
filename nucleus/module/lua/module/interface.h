/* Generated file: DO NOT EDIT ! */
#ifndef NULUA_MODULE_INTERFACE_H
#define NULUA_MODULE_INTERFACE_H

#include <nucleus/module/lua/module/definition.h>

/* loader */
#ifdef NULUA_LOADER_IMPLEMENTATION
    nulua_manager_interface_t _nulua_manager_interface;
    nu_result_t nulua_manager_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NULUA_MANAGER_INTERFACE_NAME, &_nulua_manager_interface);
    }
    nu_result_t nulua_manager_load_plugin(const char* path, nulua_plugin_t* handle)
    {
        return _nulua_manager_interface.load_plugin(path, handle);
    }
#else
    extern nulua_manager_interface_t _nulua_manager_interface;
    nu_result_t nulua_manager_interface_load(nu_module_t module);
#endif

#endif