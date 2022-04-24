#ifndef NUECS_SCENE_MANAGER_H
#define NUECS_SCENE_MANAGER_H

#include <nucleus/module/ecs/plugin/system_manager.h>
#include <nucleus/module/ecs/plugin/component_manager.h>

typedef struct {
    nu_indexed_array_t scenes;
} nuecs_scene_manager_data_t;

nu_result_t nuecs_scene_manager_initialize(nuecs_scene_manager_data_t *manager);
nu_result_t nuecs_scene_manager_terminate(nuecs_scene_manager_data_t *manager);
nu_result_t nuecs_scene_manager_progress(
    nuecs_scene_manager_data_t *scene_manager,
    nuecs_system_manager_data_t *system_manager,
    nuecs_component_manager_data_t *component_manager
);

#endif