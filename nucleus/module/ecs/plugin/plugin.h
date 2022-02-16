#ifndef NUECS_PLUGIN_H
#define NUECS_PLUGIN_H

#include <nucleus/module/ecs/module/definition.h>

/* world plugin */
nu_result_t nuecs_world_plugin_initialize(void);
nu_result_t nuecs_world_plugin_terminate(void);
nu_result_t nuecs_world_plugin_update(void);

/* world interface */
nu_result_t nuecs_world_create(nuecs_world_t* handle);
nu_result_t nuecs_world_destroy(nuecs_world_t handle);

#endif