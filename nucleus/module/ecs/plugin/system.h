#ifndef NUECS_SYSTEM_H
#define NUECS_SYSTEM_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_system_create(nuecs_system_data_t *system, nuecs_component_data_t **components, uint32_t component_count);
nu_result_t nuecs_system_destroy(nuecs_system_data_t *system);
nu_result_t nuecs_system_update(nuecs_system_data_t *system);

#endif