#ifndef NUECS_SCENE_H
#define NUECS_SCENE_H

#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/component_manager.h>

typedef struct {
    nuecs_archetype_data_t *archetype;
    nu_array_t chunks;
    nu_array_t notify_queries;
} nuecs_archetype_entry_data_t;

typedef struct {
    nu_array_t chunk_views;
    uint32_t *component_ids;
    uint32_t component_count;
    nu_result_t (*update)(nuecs_component_data_ptr_t *components, uint32_t count);
    uint32_t id;
} nuecs_system_data_t;

typedef struct {
    nu_array_t entities;        /* nuecs_entity_data_t */
    nu_array_t deleted_indices; /* uint32_t */
    nu_array_t free_indices;    /* uint32_t */
    nu_array_t archetype_table; /* nuecs_archetype_entry_t */
    nu_indexed_array_t queries; /* nuecs_query_data_t* */
    uint32_t id;    
} nuecs_scene_data_t;

nu_result_t nuecs_scene_initialize(nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_terminate(nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_progress(nuecs_scene_data_t *scene);
// nu_result_t nuecs_scene_register_system(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle);
nu_result_t nuecs_scene_create_query(nuecs_scene_data_t *scene, const nuecs_query_info_t *info, nuecs_query_t *handle);
nu_result_t nuecs_scene_destroy_query(nuecs_scene_data_t *scene, nuecs_query_t handle);
nu_result_t nuecs_scene_save_file(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const char* filename
);

#endif