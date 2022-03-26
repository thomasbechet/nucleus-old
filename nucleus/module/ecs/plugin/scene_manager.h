#ifndef NUECS_SCENE_MANAGER_H
#define NUECS_SCENE_MANAGER_H

#include <nucleus/module/ecs/plugin/scene.h>

typedef struct {
    nu_indexed_array_t scenes;
} nuecs_scene_manager_data_t;

nu_result_t nuecs_scene_manager_initialize(nuecs_scene_manager_data_t *manager);
nu_result_t nuecs_scene_manager_terminate(nuecs_scene_manager_data_t *manager);
nu_result_t nuecs_scene_manager_create_scene(nuecs_scene_manager_data_t *manager, nuecs_scene_t* handle);
nu_result_t nuecs_scene_manager_destroy_scene(nuecs_scene_manager_data_t *manager, nuecs_scene_t handle);
nu_result_t nuecs_scene_manager_progress(nuecs_scene_manager_data_t *manager);

#endif