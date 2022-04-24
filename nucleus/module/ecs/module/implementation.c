#include <nucleus/module/ecs/module/implementation.h>

#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/entity.h>
#include <nucleus/module/ecs/plugin/entity_reference.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/system_manager.h>
#include <nucleus/module/ecs/plugin/scene_manager.h>
#include <nucleus/module/ecs/plugin/query.h>
#include <nucleus/module/ecs/plugin/logger.h>
#include <nucleus/module/ecs/plugin/system.h>

#define NUECS_INVALID_SCENE_MESSAGE "Invalid scene."

typedef struct {
    nuecs_scene_manager_data_t scenes;
    nuecs_component_manager_data_t components;
    nuecs_system_manager_data_t systems;
} nuecs_module_data_t;

static nuecs_module_data_t _module;

/* archetype interface */
nu_result_t nuecs_archetype_debug_archetypes_impl(void)
{
    return NU_SUCCESS;
}
/* scene interface */
nu_result_t nuecs_scene_create_impl(nuecs_scene_t* handle)
{
    return nuecs_scene_create(&_module.scenes, handle);
}
nu_result_t nuecs_scene_destroy_impl(nuecs_scene_t handle)
{
    return nuecs_scene_destroy(&_module.scenes, handle);
}
nu_result_t nuecs_scene_clear_impl(nuecs_scene_t scene)
{
    return nuecs_scene_clear(&_module.components, (nuecs_scene_data_t*)scene);
}
nu_result_t nuecs_scene_progress_impl(nuecs_scene_t handle)
{
    return nuecs_scene_manager_progress(&_module.scenes, &_module.systems, &_module.components);
}
nu_result_t nuecs_scene_set_pipeline_impl(nuecs_scene_t scene, nuecs_pipeline_t pipeline)
{
    return nuecs_scene_set_pipeline(&_module.systems, (nuecs_scene_data_t*)scene, pipeline);
}
nu_result_t nuecs_scene_serialize_json_object_impl(nuecs_scene_t scene, nu_json_object_t object)
{
    return nuecs_scene_serialize_json_object(&_module.components, (nuecs_scene_data_t*)scene, object);
}
nu_result_t nuecs_scene_deserialize_json_object_impl(nuecs_scene_t scene, nu_json_object_t object)
{
    return nuecs_scene_deserialize_json_object(&_module.components, (nuecs_scene_data_t*)scene, object);
}
nu_result_t nuecs_scene_save_json_impl(nuecs_scene_t scene, const char* filename)
{
    return nuecs_scene_save_json(&_module.components, (nuecs_scene_data_t*)scene, filename);
}
nu_result_t nuecs_scene_load_json_impl(nuecs_scene_t scene, const char* filename)
{
    return nuecs_scene_load_json(&_module.components, (nuecs_scene_data_t*)scene, filename);
}
nu_result_t nuecs_scene_debug_entities_impl(nuecs_scene_t scene_handle)
{
    return nuecs_scene_debug_entities((nuecs_scene_data_t*)scene_handle);
}
/* query interface */
nu_result_t nuecs_query_create_impl(nuecs_scene_t scene_handle, const nuecs_query_info_t* info, nuecs_query_t* handle)
{
    return nuecs_query_create((nuecs_scene_data_t*)scene_handle, info, handle);
}
nu_result_t nuecs_query_destroy_impl(nuecs_scene_t scene_handle, nuecs_query_t handle)
{
    return nuecs_query_destroy((nuecs_scene_data_t*)scene_handle, handle);
}
nu_result_t nuecs_query_resolve_impl(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_result_t* result)
{
    return nuecs_query_resolve((nuecs_scene_data_t*)scene, (nuecs_query_data_t*)query, result);
}
/* entity interface */
nu_result_t nuecs_entity_create_impl(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
{
    return nuecs_entity_create(&_module.components, (nuecs_scene_data_t*)scene_handle, info, handle);
}
nu_result_t nuecs_entity_destroy_impl(nuecs_scene_t scene, nuecs_entity_t handle)
{
    return nuecs_entity_destroy(&_module.components, (nuecs_scene_data_t*)scene, handle);
}
nu_result_t nuecs_entity_add_component_impl(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
{
    return nuecs_entity_add_component(&_module.components, (nuecs_scene_data_t*)scene, entity, component, component_data);
}
nu_result_t nuecs_entity_remove_component_impl(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component)
{
    return nuecs_entity_remove_component(&_module.components, (nuecs_scene_data_t*)scene, entity, component);
}
nu_result_t nuecs_entity_get_component_impl(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component, nuecs_component_data_ptr_t* data)
{
    return nuecs_entity_get_component(&_module.components, (nuecs_scene_data_t*)scene, entity, component, data);
}
/* entity_reference interface */
nu_result_t nuecs_entity_reference_bind_impl(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t entity)
{
    return nuecs_entity_reference_bind((nuecs_scene_data_t*)scene, handle, entity);
}
nu_result_t nuecs_entity_reference_resolve_impl(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t* entity)
{
    return nuecs_entity_reference_resolve((nuecs_scene_data_t*)scene, handle, entity);
}
nu_result_t nuecs_entity_reference_serialize_json_object_impl(nuecs_entity_reference_t* handle, nuecs_serialization_context_t context, nu_json_object_t object, const char* name)
{
    return nuecs_entity_reference_serialize_json_object(handle, context, object, name);
}
nu_result_t nuecs_entity_reference_deserialize_json_object_impl(nuecs_deserialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_reference_t* handle)
{
    return nuecs_entity_reference_deserialize_json_object(context, object, name, handle);
}
/* component interface */
nu_result_t nuecs_component_build_impl(const nuecs_component_info_t* info, nuecs_component_t* handle)
{
    return nuecs_component_manager_build_component(&_module.components, info, handle);
}
nu_result_t nuecs_component_find_impl(const char* name, nuecs_component_t* handle)
{
    return nuecs_component_manager_find_component(&_module.components, name, handle);
}
/* system interface */
nu_result_t nuecs_system_build_impl(const nuecs_system_info_t* info, nuecs_system_t* handle)
{
    return nuecs_system_manager_build_system(&_module.systems, info, handle);
}
/* pipeline interface */
nu_result_t nuecs_pipeline_build_impl(const nuecs_pipeline_info_t* info, nuecs_pipeline_t* handle)
{
    return nuecs_system_manager_build_pipeline(&_module.systems, info, handle);
}
/* scene plugin */
nu_result_t nuecs_scene_plugin_initialize_impl(void)
{
    nuecs_component_manager_initialize(&_module.components);
    nuecs_system_manager_initialize(&_module.systems);
    nuecs_scene_manager_initialize(&_module.scenes);
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_terminate_impl(void)
{
    nuecs_scene_manager_terminate(&_module.scenes);
    nuecs_system_manager_terminate(&_module.systems);
    nuecs_component_manager_terminate(&_module.components);
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_update_impl(void)
{
    return nuecs_scene_manager_progress(&_module.scenes, &_module.systems, &_module.components);
}