/* Generated file: DO NOT EDIT ! */
#ifndef NUUTILS_MODULE_INTERFACE_H
#define NUUTILS_MODULE_INTERFACE_H

#include <nucleus/module/utils/module/definition.h>

/* loader */
#ifdef NUUTILS_LOADER_IMPLEMENTATION
    nuutils_loader_load_mesh_from_obj_pfn_t nuutils_loader_load_mesh_from_obj = NULL;
    nuutils_loader_load_texture_pfn_t nuutils_loader_load_texture = NULL;
    nu_result_t nuutils_loader_interface_load(nu_module_t module)
    {
        nuutils_loader_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUUTILS_LOADER_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuutils_loader_load_mesh_from_obj = interface.load_mesh_from_obj;
            nuutils_loader_load_texture = interface.load_texture;
        }
        return result;
    }
    nuutils_command_get_event_id_pfn_t nuutils_command_get_event_id = NULL;
    nuutils_command_execute_pfn_t nuutils_command_execute = NULL;
    nu_result_t nuutils_command_interface_load(nu_module_t module)
    {
        nuutils_command_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUUTILS_COMMAND_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuutils_command_get_event_id = interface.get_event_id;
            nuutils_command_execute = interface.execute;
        }
        return result;
    }
    nu_result_t nuutils_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nuutils_loader_interface_load(module);
        result &= nuutils_command_interface_load(module);
        return result;
    }
#else
    extern nuutils_loader_load_mesh_from_obj_pfn_t nuutils_loader_load_mesh_from_obj;
    extern nuutils_loader_load_texture_pfn_t nuutils_loader_load_texture;
    nu_result_t nuutils_loader_interface_load(nu_module_t module);
    extern nuutils_command_get_event_id_pfn_t nuutils_command_get_event_id;
    extern nuutils_command_execute_pfn_t nuutils_command_execute;
    nu_result_t nuutils_command_interface_load(nu_module_t module);
    nu_result_t nuutils_interface_load_all(nu_module_t module);
#endif

#endif