#ifndef NUECS_SCENE_H
#define NUECS_SCENE_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_scene_initialize(nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_terminate(nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_progress(nuecs_scene_t scene_handle);
nu_result_t nuecs_scene_register_system(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle);
nu_result_t nuecs_scene_create_entity(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle);
nu_result_t nuecs_scene_destroy_entity(nuecs_scene_t scene_handle, nuecs_entity_t handle);
nu_result_t nuecs_scene_entity_add_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data);
nu_result_t nuecs_scene_entity_remove_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component);
nu_result_t nuecs_scene_create_query(nuecs_scene_t scene_handle, const nuecs_query_info_t *info, nuecs_query_t *handle);
nu_result_t nuecs_scene_destroy_query(nuecs_scene_t scene_handle, nuecs_query_t handle);
nu_result_t nuecs_scene_save_file(nuecs_scene_t scene_handle, const char* filename);

#endif