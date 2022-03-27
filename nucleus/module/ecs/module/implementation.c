#include <nucleus/module/ecs/module/implementation.h>

#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/entity.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/scene_manager.h>
#include <nucleus/module/ecs/plugin/query.h>

typedef struct {
     nuecs_scene_manager_data_t scenes;
     nuecs_component_manager_data_t components;
} nuecs_module_data_t;

static nuecs_module_data_t _module;

/* archetype interface */
nu_result_t nuecs_archetype_debug_archetypes_impl(void)
{
    return nuecs_component_manager_debug_archetypes(&_module.components);
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
nu_result_t nuecs_scene_progress_impl(nuecs_scene_t handle)
{
    return nuecs_scene_manager_progress(&_module.scenes);
}
nu_result_t nuecs_scene_register_system_impl(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_save_file_impl(nuecs_scene_t scene_handle, const char* filename)
{
    return nuecs_scene_save_file(&_module.components, (nuecs_scene_data_t*)scene_handle, filename);
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
nu_result_t nuecs_query_resolve_chunks_impl(nuecs_query_t handle, nuecs_query_chunks_t* chunks)
{
    return nuecs_query_resolve_chunks((nuecs_query_data_t*)handle, chunks);
}
/* entity interface */
nu_result_t nuecs_entity_create_impl(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
{
    return nuecs_entity_create(&_module.components, (nuecs_scene_data_t*)scene_handle, info, handle);
}
nu_result_t nuecs_entity_destroy_impl(nuecs_scene_t scene_handle, nuecs_entity_t handle)
{
    return nuecs_entity_destroy((nuecs_scene_data_t*)scene_handle, handle);
}
nu_result_t nuecs_entity_add_component_impl(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
{
    return nuecs_entity_add_component(&_module.components, (nuecs_scene_data_t*)scene_handle, handle, component, component_data);
}
nu_result_t nuecs_entity_remove_component_impl(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component)
{
    return nuecs_entity_remove_component(&_module.components, (nuecs_scene_data_t*)scene_handle, handle, component);
}
nu_result_t nuecs_entity_serialize_json_object_impl(nuecs_entity_t handle, nuecs_serialization_context_t context, nu_json_object_t object, const char* name)
{
    return nuecs_entity_serialize_json_object(handle, context, object, name);
}
nu_result_t nuecs_entity_deserialize_json_object_impl(nuecs_serialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_t* handle)
{
    return nuecs_entity_deserialize_json_object(context, object, name, handle);
}
nu_result_t nuecs_entity_remap_impl(nuecs_transfer_context_t context, nuecs_entity_t* handle)
{
    return nuecs_entity_remap(context, handle);
}
/* component interface */
nu_result_t nuecs_component_record_impl(const nuecs_component_info_t* info, nuecs_component_t* handle)
{
    return nuecs_component_manager_record_component(&_module.components, info, handle);
}
/* scene plugin */
nu_result_t nuecs_scene_plugin_initialize_impl(void)
{
    nuecs_component_manager_initialize(&_module.components);
    nuecs_scene_manager_initialize(&_module.scenes);
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_terminate_impl(void)
{
    nuecs_scene_manager_terminate(&_module.scenes);
    nuecs_component_manager_terminate(&_module.components);
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_update_impl(void)
{
    return nuecs_scene_manager_progress(&_module.scenes);
    return NU_SUCCESS;
}