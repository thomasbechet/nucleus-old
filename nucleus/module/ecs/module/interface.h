/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_INTERFACE_H
#define NUECS_MODULE_INTERFACE_H

#include <nucleus/module/ecs/module/definition.h>

/* loader */
#ifdef NUECS_LOADER_IMPLEMENTATION
    nuecs_manager_interface_t _nuecs_manager_interface;
    nu_result_t nuecs_manager_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_MANAGER_INTERFACE_NAME, &_nuecs_manager_interface);
    }
    nu_result_t nuecs_manager_register_component(const nuecs_component_info_t* info, nuecs_component_t* handle)
    {
        return _nuecs_manager_interface.register_component(info, handle);
    }
    nu_result_t nuecs_manager_create_scene(nuecs_scene_t* handle)
    {
        return _nuecs_manager_interface.create_scene(handle);
    }
    nu_result_t nuecs_manager_destroy_scene(nuecs_scene_t handle)
    {
        return _nuecs_manager_interface.destroy_scene(handle);
    }
    nuecs_scene_interface_t _nuecs_scene_interface;
    nu_result_t nuecs_scene_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_SCENE_INTERFACE_NAME, &_nuecs_scene_interface);
    }
    nu_result_t nuecs_scene_progress(nuecs_scene_t handle)
    {
        return _nuecs_scene_interface.progress(handle);
    }
    nu_result_t nuecs_scene_register_system(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle)
    {
        return _nuecs_scene_interface.register_system(scene_handle, info, handle);
    }
    nu_result_t nuecs_scene_create_entity(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
    {
        return _nuecs_scene_interface.create_entity(scene_handle, info, handle);
    }
    nu_result_t nuecs_scene_destroy_entity(nuecs_scene_t scene_handle, nuecs_entity_t handle)
    {
        return _nuecs_scene_interface.destroy_entity(scene_handle, handle);
    }
    nu_result_t nuecs_scene_entity_add_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
    {
        return _nuecs_scene_interface.entity_add_component(scene_handle, handle, component, component_data);
    }
    nu_result_t nuecs_scene_entity_remove_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component)
    {
        return _nuecs_scene_interface.entity_remove_component(scene_handle, handle, component);
    }
    nu_result_t nuecs_scene_save_file(nuecs_scene_t scene_handle, const char* filename)
    {
        return _nuecs_scene_interface.save_file(scene_handle, filename);
    }
    nuecs_query_interface_t _nuecs_query_interface;
    nu_result_t nuecs_query_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_QUERY_INTERFACE_NAME, &_nuecs_query_interface);
    }
    nu_result_t nuecs_query_create(nuecs_scene_t scene_handle, const nuecs_query_info_t* info, nuecs_query_t* handle)
    {
        return _nuecs_query_interface.create(scene_handle, info, handle);
    }
    nu_result_t nuecs_query_destroy(nuecs_scene_t scene_handle, nuecs_query_t handle)
    {
        return _nuecs_query_interface.destroy(scene_handle, handle);
    }
    nu_result_t nuecs_query_resolve_chunks(nuecs_scene_t scene_handle, nuecs_query_t handle, nuecs_query_chunks_t* chunks)
    {
        return _nuecs_query_interface.resolve_chunks(scene_handle, handle, chunks);
    }
#else
    extern nuecs_manager_interface_t _nuecs_manager_interface;
    nu_result_t nuecs_manager_interface_load(nu_module_t module);
    extern nuecs_scene_interface_t _nuecs_scene_interface;
    nu_result_t nuecs_scene_interface_load(nu_module_t module);
    extern nuecs_query_interface_t _nuecs_query_interface;
    nu_result_t nuecs_query_interface_load(nu_module_t module);
#endif

#endif