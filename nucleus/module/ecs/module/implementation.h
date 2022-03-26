/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_IMPLEMENTATION_H
#define NUECS_IMPLEMENTATION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/ecs/plugin/types_public.h>

/* archetype interface */
nu_result_t nuecs_archetype_debug_archetypes_impl(void);
/* scene interface */
nu_result_t nuecs_scene_create_impl(nuecs_scene_t* handle);
nu_result_t nuecs_scene_destroy_impl(nuecs_scene_t handle);
nu_result_t nuecs_scene_progress_impl(nuecs_scene_t handle);
nu_result_t nuecs_scene_register_system_impl(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle);
nu_result_t nuecs_scene_save_file_impl(nuecs_scene_t scene_handle, const char* filename);
/* query interface */
nu_result_t nuecs_query_create_impl(nuecs_scene_t scene_handle, const nuecs_query_info_t* info, nuecs_query_t* handle);
nu_result_t nuecs_query_destroy_impl(nuecs_scene_t scene_handle, nuecs_query_t handle);
nu_result_t nuecs_query_resolve_chunks_impl(nuecs_query_t handle, nuecs_query_chunks_t* chunks);
/* entity interface */
nu_result_t nuecs_entity_create_impl(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle);
nu_result_t nuecs_entity_destroy_impl(nuecs_scene_t scene_handle, nuecs_entity_t handle);
nu_result_t nuecs_entity_add_component_impl(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data);
nu_result_t nuecs_entity_remove_component_impl(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component);
/* component interface */
nu_result_t nuecs_component_record_impl(const nuecs_component_info_t* info, nuecs_component_t* handle);
/* scene plugin */
nu_result_t nuecs_scene_plugin_initialize_impl(void);
nu_result_t nuecs_scene_plugin_terminate_impl(void);
nu_result_t nuecs_scene_plugin_update_impl(void);

#endif