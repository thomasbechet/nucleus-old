#ifndef NUECS_WORLD_H
#define NUECS_WORLD_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world);
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world);
nu_result_t nuecs_world_register_component(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle);
nu_result_t nuecs_world_register_system(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle);

#endif