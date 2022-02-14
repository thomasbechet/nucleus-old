/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_INTERFACE_H
#define NUECS_MODULE_INTERFACE_H

#include <nucleus/module/ecs/module/definition.h>

/* loader */
#ifdef NUECS_BOOTSTRAP
    nuecs_world_interface_t _nuecs_world_interface;
    nu_result_t nuecs_world_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_WORLD_INTERFACE_NAME, &_nuecs_world_interface);
    }
    nu_result_t nuecs_world_create(nuecs_world_t* handle)
    {
        return _nuecs_world_interface.create(handle);
    }
    nu_result_t nuecs_world_destroy(nuecs_world_t handle)
    {
        return _nuecs_world_interface.destroy(handle);
    }
    nu_result_t nuecs_world_progress(nuecs_world_t handle)
    {
        return _nuecs_world_interface.progress(handle);
    }
    nu_result_t nuecs_world_register_component(nuecs_world_t world_handle, const nuecs_component_info_t* info, nuecs_component_t* handle)
    {
        return _nuecs_world_interface.register_component(world_handle, info, handle);
    }
    nu_result_t nuecs_world_register_system(nuecs_world_t world_handle, const nuecs_system_info_t* info, nuecs_system_t* handle)
    {
        return _nuecs_world_interface.register_system(world_handle, info, handle);
    }
    nu_result_t nuecs_world_create_entity(nuecs_world_t world_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
    {
        return _nuecs_world_interface.create_entity(world_handle, info, handle);
    }
    nu_result_t nuecs_world_destroy_entity(nuecs_world_t world_handle, nuecs_entity_t handle)
    {
        return _nuecs_world_interface.destroy_entity(world_handle, handle);
    }
    nu_result_t nuecs_world_entity_add_component(nuecs_world_t world_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
    {
        return _nuecs_world_interface.entity_add_component(world_handle, handle, component, component_data);
    }
    nu_result_t nuecs_world_entity_remove_component(nuecs_world_t world_handle, nuecs_entity_t handle, nuecs_component_t component)
    {
        return _nuecs_world_interface.entity_remove_component(world_handle, handle, component);
    }
#else
    extern nuecs_world_interface_t _nuecs_world_interface;
    nu_result_t nuecs_world_interface_load(nu_module_t module);
#endif

#endif