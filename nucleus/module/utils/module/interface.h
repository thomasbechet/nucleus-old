/* Generated file: DO NOT EDIT ! */
#ifndef NUUTILS_MODULE_INTERFACE_H
#define NUUTILS_MODULE_INTERFACE_H

#include <nucleus/module/utils/module/definition.h>

/* loader */
#ifdef NUUTILS_LOADER_IMPLEMENTATION
    nuutils_loader_interface_t _nuutils_loader_interface;
    nu_result_t nuutils_loader_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUUTILS_LOADER_INTERFACE_NAME, &_nuutils_loader_interface);
    }
    nu_result_t nuutils_loader_load_mesh_from_obj(const char* path, nu_renderer_mesh_t* handle)
    {
        return _nuutils_loader_interface.load_mesh_from_obj(path, handle);
    }
    nu_result_t nuutils_loader_load_texture(const char* path, nu_renderer_texture_t* handle)
    {
        return _nuutils_loader_interface.load_texture(path, handle);
    }
    nuutils_command_interface_t _nuutils_command_interface;
    nu_result_t nuutils_command_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUUTILS_COMMAND_INTERFACE_NAME, &_nuutils_command_interface);
    }
    nu_result_t nuutils_command_get_event_id(nu_event_id_t* id)
    {
        return _nuutils_command_interface.get_event_id(id);
    }
    nu_result_t nuutils_command_execute(const char* command)
    {
        return _nuutils_command_interface.execute(command);
    }
    nu_result_t nuutils_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nuutils_loader_interface_load(module);
        result &= nuutils_command_interface_load(module);
        return result;
    }
#else
    extern nuutils_loader_interface_t _nuutils_loader_interface;
    nu_result_t nuutils_loader_interface_load(nu_module_t module);
    extern nuutils_command_interface_t _nuutils_command_interface;
    nu_result_t nuutils_command_interface_load(nu_module_t module);
    nu_result_t nuutils_interface_load_all(nu_module_t module);
#endif

#endif