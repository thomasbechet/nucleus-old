#ifndef NUECS_WORLD_H
#define NUECS_WORLD_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world);
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world);
nu_result_t nuecs_world_progress(nuecs_world_t handle);
nu_result_t nuecs_world_register_component(nuecs_world_t world_handle, const nuecs_component_info_t* info, nuecs_component_t* handle);
nu_result_t nuecs_world_register_system(nuecs_world_t world_handle, const nuecs_system_info_t* info, nuecs_system_t* handle);
nu_result_t nuecs_world_create_entity(nuecs_world_t world_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle);
nu_result_t nuecs_world_destroy_entity(nuecs_world_t world_handle, nuecs_entity_t handle);
nu_result_t nuecs_world_entity_add_component(nuecs_world_t world_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t component_data);
nu_result_t nuecs_world_entity_remove_component(nuecs_world_t world_handle, nuecs_entity_t handle, nuecs_component_t component);

#endif