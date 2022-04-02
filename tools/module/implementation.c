#include <nucleus/module/ecs/module/implementation.h>

/* archetype interface */
nu_result_t nuecs_archetype_debug_archetypes_impl(void)
{
    return NU_SUCCESS;
}
/* scene interface */
nu_result_t nuecs_scene_create_impl(nuecs_scene_t* scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_destroy_impl(nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_clear_impl(nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_progress_impl(nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_register_system_impl(nuecs_scene_t scene, const nuecs_system_info_t* info, nuecs_system_t* handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_serialize_json_object_impl(nuecs_scene_t scene, nu_json_object_t object)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_deserialize_json_object_impl(nuecs_scene_t scene, nu_json_object_t object)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_save_json_impl(nuecs_scene_t scene, const char* filename)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_load_json_impl(nuecs_scene_t scene, const char* filename)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_debug_entities_impl(nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
/* query interface */
nu_result_t nuecs_query_create_impl(nuecs_scene_t scene, const nuecs_query_info_t* info, nuecs_query_t* handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_query_destroy_impl(nuecs_scene_t scene, nuecs_query_t query)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_query_resolve_chunks_impl(nuecs_query_t query, nuecs_query_chunks_t* chunks)
{
    return NU_SUCCESS;
}
/* entity interface */
nu_result_t nuecs_entity_create_impl(nuecs_scene_t scene, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_entity_destroy_impl(nuecs_scene_t scene, nuecs_entity_t entity)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_entity_add_component_impl(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component, nuecs_component_data_ptr_t component_data)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_entity_remove_component_impl(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_entity_serialize_json_object_impl(nuecs_entity_t entity, nuecs_serialization_context_t context, nu_json_object_t object, const char* name)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_entity_deserialize_json_object_impl(nuecs_deserialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_t* handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_entity_remap_impl(nuecs_transfer_context_t context, nuecs_entity_t* handle)
{
    return NU_SUCCESS;
}
/* component interface */
nu_result_t nuecs_component_record_impl(const nuecs_component_info_t* info, nuecs_component_t* handle)
{
    return NU_SUCCESS;
}
/* scene plugin */
nu_result_t nuecs_scene_plugin_initialize_impl(void)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_terminate_impl(void)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_update_impl(void)
{
    return NU_SUCCESS;
}