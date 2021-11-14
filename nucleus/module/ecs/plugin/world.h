#ifndef NUECS_WORLD_H
#define NUECS_WORLD_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world);
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world);
nu_result_t nuecs_world_component_register(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle);
nu_result_t nuecs_world_system_register(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle);
nu_result_t nuecs_world_entity_create(nuecs_world_data_t *world, const nuecs_entity_info_t *info, nuecs_entity_t *handle);
nu_result_t nuecs_world_entity_destroy(nuecs_world_data_t *world, nuecs_entity_t handle);

#endif