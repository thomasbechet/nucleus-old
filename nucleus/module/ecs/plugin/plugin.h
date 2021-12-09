#ifndef NUECS_PLUGIN_H
#define NUECS_PLUGIN_H

#include <nucleus/module/ecs/module/interface.h>

/* plugin interface */
nu_result_t nuecs_plugin_initialize(void);
nu_result_t nuecs_plugin_terminate(void);
nu_result_t nuecs_plugin_update(void);

/* ecs plugin interface */
nu_result_t nuecs_world_create(nuecs_world_t *handle);
nu_result_t nuecs_world_progress(nuecs_world_t world);
nu_result_t nuecs_component_register(nuecs_world_t world, const nuecs_component_info_t *info, nuecs_component_t *handle);
nu_result_t nuecs_system_register(nuecs_world_t world, const nuecs_system_info_t *info, nuecs_system_t *handle);
nu_result_t nuecs_entity_create(nuecs_world_t world, const nuecs_entity_info_t *info, nuecs_entity_t *handle);
nu_result_t nuecs_entity_destroy(nuecs_world_t world, nuecs_entity_t entity);

#endif