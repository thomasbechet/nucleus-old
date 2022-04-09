/* Generated file: DO NOT EDIT ! */
#ifndef NUUTILS_MODULE_INTERFACE_H
#define NUUTILS_MODULE_INTERFACE_H

#include <nucleus/module/utils/module/definition.h>

/* functions definition */
typedef nu_result_t (*__nuutils_loader_load_mesh_from_obj_pfn_t)(const char* path, nu_renderer_mesh_t* handle);
typedef nu_result_t (*__nuutils_loader_load_texture_pfn_t)(const char* path, nu_renderer_texture_t* handle);
typedef nu_result_t (*__nuutils_command_get_event_id_pfn_t)(nu_event_id_t* id);
typedef nu_result_t (*__nuutils_command_execute_pfn_t)(const char* command);

/* loader */
#ifdef NUUTILS_LOADER_IMPLEMENTATION
    __nuutils_loader_load_mesh_from_obj_pfn_t __nuutils_loader_load_mesh_from_obj = NULL;
    __nuutils_loader_load_texture_pfn_t __nuutils_loader_load_texture = NULL;
    nu_result_t nuutils_loader_interface_load(nu_module_t module)
    {
        nuutils_loader_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUUTILS_LOADER_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuutils_loader_load_mesh_from_obj = interface.load_mesh_from_obj;
            __nuutils_loader_load_texture = interface.load_texture;
        }
        return result;
    }
    __nuutils_command_get_event_id_pfn_t __nuutils_command_get_event_id = NULL;
    __nuutils_command_execute_pfn_t __nuutils_command_execute = NULL;
    nu_result_t nuutils_command_interface_load(nu_module_t module)
    {
        nuutils_command_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUUTILS_COMMAND_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuutils_command_get_event_id = interface.get_event_id;
            __nuutils_command_execute = interface.execute;
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
    extern __nuutils_loader_load_mesh_from_obj_pfn_t __nuutils_loader_load_mesh_from_obj;
    extern __nuutils_loader_load_texture_pfn_t __nuutils_loader_load_texture;
    nu_result_t nuutils_loader_interface_load(nu_module_t module);
    extern __nuutils_command_get_event_id_pfn_t __nuutils_command_get_event_id;
    extern __nuutils_command_execute_pfn_t __nuutils_command_execute;
    nu_result_t nuutils_command_interface_load(nu_module_t module);
    nu_result_t nuutils_interface_load_all(nu_module_t module);
#endif
#define nuutils_loader_load_mesh_from_obj NU_IDENTITY(__nuutils_loader_load_mesh_from_obj)
#define nuutils_loader_load_texture NU_IDENTITY(__nuutils_loader_load_texture)
#define nuutils_command_get_event_id NU_IDENTITY(__nuutils_command_get_event_id)
#define nuutils_command_execute NU_IDENTITY(__nuutils_command_execute)

#endif