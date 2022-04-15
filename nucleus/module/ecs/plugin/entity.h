#ifndef NUECS_ENTITY_H
#define NUECS_ENTITY_H

#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/scene.h>

nu_result_t nuecs_entity_create(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const nuecs_entity_info_t* info, 
    nuecs_entity_t *handle
);
nu_result_t nuecs_entity_destroy(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle
);
nu_result_t nuecs_entity_add(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    uint32_t *components,
    uint32_t component_count,
    nuecs_entity_t *handle
);
nu_result_t nuecs_entity_remove(
    nuecs_scene_data_t *scene,
    nuecs_entity_t handle
);
nu_result_t nuecs_entity_add_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    nuecs_entity_t *handle,
    nuecs_component_t component,
    nuecs_component_data_ptr_t data
);
nu_result_t nuecs_entity_remove_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    nuecs_entity_t *handle,
    nuecs_component_t component
);
nu_result_t nuecs_entity_get_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    nuecs_entity_t handle,
    nuecs_component_t component,
    nuecs_component_data_ptr_t *data
);

#endif