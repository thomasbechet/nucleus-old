/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_INTERFACE_H
#define NUECS_MODULE_INTERFACE_H

#include <nucleus/module/ecs/module/definition.h>

/* loader */
#ifdef NUECS_LOADER_IMPLEMENTATION
    nuecs_archetype_interface_t _nuecs_archetype_interface;
    nu_result_t nuecs_archetype_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_ARCHETYPE_INTERFACE_NAME, &_nuecs_archetype_interface);
    }
    nu_result_t nuecs_archetype_debug_archetypes(void)
    {
        return _nuecs_archetype_interface.debug_archetypes();
    }
    nuecs_scene_interface_t _nuecs_scene_interface;
    nu_result_t nuecs_scene_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_SCENE_INTERFACE_NAME, &_nuecs_scene_interface);
    }
    nu_result_t nuecs_scene_create(nuecs_scene_t* handle)
    {
        return _nuecs_scene_interface.create(handle);
    }
    nu_result_t nuecs_scene_destroy(nuecs_scene_t handle)
    {
        return _nuecs_scene_interface.destroy(handle);
    }
    nu_result_t nuecs_scene_progress(nuecs_scene_t handle)
    {
        return _nuecs_scene_interface.progress(handle);
    }
    nu_result_t nuecs_scene_register_system(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle)
    {
        return _nuecs_scene_interface.register_system(scene_handle, info, handle);
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
    nu_result_t nuecs_query_resolve_chunks(nuecs_query_t handle, nuecs_query_chunks_t* chunks)
    {
        return _nuecs_query_interface.resolve_chunks(handle, chunks);
    }
    nuecs_entity_interface_t _nuecs_entity_interface;
    nu_result_t nuecs_entity_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_ENTITY_INTERFACE_NAME, &_nuecs_entity_interface);
    }
    nu_result_t nuecs_entity_create(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
    {
        return _nuecs_entity_interface.create(scene_handle, info, handle);
    }
    nu_result_t nuecs_entity_destroy(nuecs_scene_t scene_handle, nuecs_entity_t handle)
    {
        return _nuecs_entity_interface.destroy(scene_handle, handle);
    }
    nu_result_t nuecs_entity_add_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
    {
        return _nuecs_entity_interface.add_component(scene_handle, handle, component, component_data);
    }
    nu_result_t nuecs_entity_remove_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component)
    {
        return _nuecs_entity_interface.remove_component(scene_handle, handle, component);
    }
    nuecs_component_interface_t _nuecs_component_interface;
    nu_result_t nuecs_component_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_COMPONENT_INTERFACE_NAME, &_nuecs_component_interface);
    }
    nu_result_t nuecs_component_record(const nuecs_component_info_t* info, nuecs_component_t* handle)
    {
        return _nuecs_component_interface.record(info, handle);
    }
    nu_result_t nuecs_interface_load_all(nu_module_t module)
    {
        nuecs_archetype_interface_load(module);
        nuecs_scene_interface_load(module);
        nuecs_query_interface_load(module);
        nuecs_entity_interface_load(module);
        nuecs_component_interface_load(module);
    }
#else
    extern nuecs_archetype_interface_t _nuecs_archetype_interface;
    nu_result_t nuecs_archetype_interface_load(nu_module_t module);
    extern nuecs_scene_interface_t _nuecs_scene_interface;
    nu_result_t nuecs_scene_interface_load(nu_module_t module);
    extern nuecs_query_interface_t _nuecs_query_interface;
    nu_result_t nuecs_query_interface_load(nu_module_t module);
    extern nuecs_entity_interface_t _nuecs_entity_interface;
    nu_result_t nuecs_entity_interface_load(nu_module_t module);
    extern nuecs_component_interface_t _nuecs_component_interface;
    nu_result_t nuecs_component_interface_load(nu_module_t module);
    nu_result_t nuecs_interface_load_all(nu_module_t module);
#endif

#endif