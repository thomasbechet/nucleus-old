/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_IMPLEMENTATION_H
#define NUECS_IMPLEMENTATION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/ecs/plugin/types_public.h>

/* archetype interface */
nu_result_t nuecs_archetype_debug_archetypes_impl(void);
/* scene interface */
nu_result_t nuecs_scene_create_impl(nuecs_scene_t* scene);
nu_result_t nuecs_scene_destroy_impl(nuecs_scene_t scene);
nu_result_t nuecs_scene_clear_impl(nuecs_scene_t scene);
nu_result_t nuecs_scene_set_pipeline_impl(nuecs_scene_t scene, nuecs_pipeline_t pipeline);
nu_result_t nuecs_scene_progress_impl(nuecs_scene_t scene);
nu_result_t nuecs_scene_serialize_json_object_impl(nuecs_scene_t scene, nu_json_object_t object);
nu_result_t nuecs_scene_deserialize_json_object_impl(nuecs_scene_t scene, nu_json_object_t object);
nu_result_t nuecs_scene_save_json_impl(nuecs_scene_t scene, const char* filename);
nu_result_t nuecs_scene_load_json_impl(nuecs_scene_t scene, const char* filename);
nu_result_t nuecs_scene_debug_entities_impl(nuecs_scene_t scene);
/* query interface */
nu_result_t nuecs_query_create_impl(nuecs_scene_t scene, const nuecs_query_info_t* info, nuecs_query_t* handle);
nu_result_t nuecs_query_destroy_impl(nuecs_scene_t scene, nuecs_query_t query);
nu_result_t nuecs_query_resolve_impl(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_result_t* result);
/* entity interface */
nu_result_t nuecs_entity_create_impl(nuecs_scene_t scene, const nuecs_entity_info_t* info, nuecs_entity_t* handle);
nu_result_t nuecs_entity_destroy_impl(nuecs_scene_t scene, nuecs_entity_t entity);
nu_result_t nuecs_entity_add_component_impl(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component, nuecs_component_data_ptr_t component_data);
nu_result_t nuecs_entity_remove_component_impl(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component);
nu_result_t nuecs_entity_get_component_impl(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component, nuecs_component_data_ptr_t* data);
/* entity_reference interface */
nu_result_t nuecs_entity_reference_bind_impl(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t entity);
nu_result_t nuecs_entity_reference_resolve_impl(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t* entity);
nu_result_t nuecs_entity_reference_serialize_json_object_impl(nuecs_entity_reference_t* handle, nuecs_serialization_context_t context, nu_json_object_t object, const char* name);
nu_result_t nuecs_entity_reference_deserialize_json_object_impl(nuecs_deserialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_reference_t* handle);
/* component interface */
nu_result_t nuecs_component_build_impl(const nuecs_component_info_t* info, nuecs_component_t* handle);
nu_result_t nuecs_component_find_impl(const char* name, nuecs_component_t* handle);
/* system interface */
nu_result_t nuecs_system_build_impl(const nuecs_system_info_t* info, nuecs_system_t* handle);
/* pipeline interface */
nu_result_t nuecs_pipeline_build_impl(const nuecs_pipeline_info_t* info, nuecs_pipeline_t* handle);
/* scene plugin */
nu_result_t nuecs_scene_plugin_initialize_impl(void);
nu_result_t nuecs_scene_plugin_terminate_impl(void);
nu_result_t nuecs_scene_plugin_update_impl(void);

#endif