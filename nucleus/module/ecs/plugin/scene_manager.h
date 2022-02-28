#ifndef NUECS_SCENE_MANAGER_H
#define NUECS_SCENE_MANAGER_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_scene_manager_initialize(void);
nu_result_t nuecs_scene_manager_terminate(void);
nu_result_t nuecs_scene_manager_create_scene(nuecs_scene_t* handle);
nu_result_t nuecs_scene_manager_destroy_scene(nuecs_scene_t handle);
nu_result_t nuecs_scene_manager_progress(void);

#endif