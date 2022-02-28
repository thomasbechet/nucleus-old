#ifndef NUECS_PLUGIN_H
#define NUECS_PLUGIN_H

#include <nucleus/module/ecs/module/definition.h>

/* scene plugin */
nu_result_t nuecs_scene_plugin_initialize(void);
nu_result_t nuecs_scene_plugin_terminate(void);
nu_result_t nuecs_scene_plugin_update(void);

#endif