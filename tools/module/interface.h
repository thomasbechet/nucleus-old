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
    nu_result_t nuecs_scene_create(nuecs_scene_t* scene)
    {
        return _nuecs_scene_interface.create(scene);
    }
    nu_result_t nuecs_scene_destroy(nuecs_scene_t scene)
    {
        return _nuecs_scene_interface.destroy(scene);
    }
    nu_result_t nuecs_scene_clear(nuecs_scene_t scene)
    {
        return _nuecs_scene_interface.clear(scene);
    }
    nu_result_t nuecs_scene_progress(nuecs_scene_t scene)
    {
        return _nuecs_scene_interface.progress(scene);
    }
    nu_result_t nuecs_scene_set_pipeline(nuecs_scene_t scene, nuecs_pipeline_t pipeline)
    {
        return _nuecs_scene_interface.set_pipeline(scene, pipeline);
    }
    nu_result_t nuecs_scene_serialize_json_object(nuecs_scene_t scene, nu_json_object_t object)
    {
        return _nuecs_scene_interface.serialize_json_object(scene, object);
    }
    nu_result_t nuecs_scene_deserialize_json_object(nuecs_scene_t scene, nu_json_object_t object)
    {
        return _nuecs_scene_interface.deserialize_json_object(scene, object);
    }
    nu_result_t nuecs_scene_save_json(nuecs_scene_t scene, const char* filename)
    {
        return _nuecs_scene_interface.save_json(scene, filename);
    }
    nu_result_t nuecs_scene_load_json(nuecs_scene_t scene, const char* filename)
    {
        return _nuecs_scene_interface.load_json(scene, filename);
    }
    nu_result_t nuecs_scene_debug_entities(nuecs_scene_t scene)
    {
        return _nuecs_scene_interface.debug_entities(scene);
    }
    nuecs_query_interface_t _nuecs_query_interface;
    nu_result_t nuecs_query_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_QUERY_INTERFACE_NAME, &_nuecs_query_interface);
    }
    nu_result_t nuecs_query_create(nuecs_scene_t scene, const nuecs_query_info_t* info, nuecs_query_t* handle)
    {
        return _nuecs_query_interface.create(scene, info, handle);
    }
    nu_result_t nuecs_query_destroy(nuecs_scene_t scene, nuecs_query_t query)
    {
        return _nuecs_query_interface.destroy(scene, query);
    }
    nu_result_t nuecs_query_resolve_chunks(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_chunks_t* chunks)
    {
        return _nuecs_query_interface.resolve_chunks(scene, query, chunks);
    }
    nuecs_entity_interface_t _nuecs_entity_interface;
    nu_result_t nuecs_entity_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_ENTITY_INTERFACE_NAME, &_nuecs_entity_interface);
    }
    nu_result_t nuecs_entity_create(nuecs_scene_t scene, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
    {
        return _nuecs_entity_interface.create(scene, info, handle);
    }
    nu_result_t nuecs_entity_destroy(nuecs_scene_t scene, nuecs_entity_t entity)
    {
        return _nuecs_entity_interface.destroy(scene, entity);
    }
    nu_result_t nuecs_entity_add_component(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
    {
        return _nuecs_entity_interface.add_component(scene, entity, component, component_data);
    }
    nu_result_t nuecs_entity_remove_component(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component)
    {
        return _nuecs_entity_interface.remove_component(scene, entity, component);
    }
    nu_result_t nuecs_entity_serialize_json_object(nuecs_entity_t entity, nuecs_serialization_context_t context, nu_json_object_t object, const char* name)
    {
        return _nuecs_entity_interface.serialize_json_object(entity, context, object, name);
    }
    nu_result_t nuecs_entity_deserialize_json_object(nuecs_deserialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_t* handle)
    {
        return _nuecs_entity_interface.deserialize_json_object(context, object, name, handle);
    }
    nu_result_t nuecs_entity_remap(nuecs_transfer_context_t context, nuecs_entity_t* handle)
    {
        return _nuecs_entity_interface.remap(context, handle);
    }
    nuecs_component_interface_t _nuecs_component_interface;
    nu_result_t nuecs_component_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_COMPONENT_INTERFACE_NAME, &_nuecs_component_interface);
    }
    nu_result_t nuecs_component_build(const nuecs_component_info_t* info, nuecs_component_t* handle)
    {
        return _nuecs_component_interface.build(info, handle);
    }
    nuecs_system_interface_t _nuecs_system_interface;
    nu_result_t nuecs_system_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUECS_SYSTEM_INTERFACE_NAME, &_nuecs_system_interface);
    }
    nu_result_t nuecs_system_build(nuecs_system_info_t* info, nuecs_system_t* handle)
    {
        return _nuecs_system_interface.build(info, handle);
    }
    nu_result_t nuecs_system_compile_pipeline(nuecs_pipeline_info_t* info, nuecs_pipeline_t* handle)
    {
        return _nuecs_system_interface.compile_pipeline(info, handle);
    }
    nu_result_t nuecs_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nuecs_archetype_interface_load(module);
        result &= nuecs_scene_interface_load(module);
        result &= nuecs_query_interface_load(module);
        result &= nuecs_entity_interface_load(module);
        result &= nuecs_component_interface_load(module);
        result &= nuecs_system_interface_load(module);
        return result;
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
    extern nuecs_system_interface_t _nuecs_system_interface;
    nu_result_t nuecs_system_interface_load(nu_module_t module);
    nu_result_t nuecs_interface_load_all(nu_module_t module);
#endif

#endif