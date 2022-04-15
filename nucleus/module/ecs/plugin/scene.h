#ifndef NUECS_SCENE_H
#define NUECS_SCENE_H

#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/scene_manager.h>
#include <nucleus/module/ecs/plugin/system_manager.h>
#include <nucleus/module/ecs/plugin/chunk_table.h>
#include <nucleus/module/ecs/plugin/pipeline.h>

typedef struct {
    nu_array_t entities_to_delete;   /* nuecs_entity_t */
    nu_array_t chunks;               /* nuecs_chunk_data_t* */
    nuecs_chunk_table_t chunk_table; /* nuecs_archetype_entry_t */
    nu_indexed_array_t queries;      /* nuecs_query_data_t* */
    nu_array_t references;           /* nuecs_entity_reference_data_t */
    nu_array_t free_references;      /* uint32_t */
    uint8_t next_reference_version;
    nuecs_pipeline_t next_pipeline;
    nuecs_pipeline_instance_data_t pipeline_instance;
    bool destroy;
    uint32_t id;
} nuecs_scene_data_t;

nu_result_t nuecs_scene_create(nuecs_scene_manager_data_t *manager, nuecs_scene_t *handle);
nu_result_t nuecs_scene_destroy(nuecs_scene_manager_data_t *manager, nuecs_scene_t handle);
nu_result_t nuecs_scene_initialize(nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_terminate(nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_clear(nuecs_component_manager_data_t *manager, nuecs_scene_data_t *scene);
nu_result_t nuecs_scene_set_pipeline(
    nuecs_system_manager_data_t *system_manager,
    nuecs_scene_data_t *scene,
    nuecs_pipeline_t pipeline
);
nu_result_t nuecs_scene_progress(
    nuecs_system_manager_data_t *system_manager,
    nuecs_component_manager_data_t *component_manager,
    nuecs_scene_data_t *scene
);
nu_result_t nuecs_scene_serialize_json_object(
    nuecs_component_manager_data_t *manager, 
    nuecs_scene_data_t *scene,
    nu_json_object_t object
);
nu_result_t nuecs_scene_deserialize_json_object(
    nuecs_component_manager_data_t *manager, 
    nuecs_scene_data_t *scene,
    nu_json_object_t object
);
nu_result_t nuecs_scene_save_json(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    const char* filename
);
nu_result_t nuecs_scene_load_json(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    const char* filename
);
nu_result_t nuecs_scene_debug_entities(nuecs_scene_data_t *scene);

#endif