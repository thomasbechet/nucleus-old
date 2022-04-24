#ifndef NUECS_PIPELINE_H
#define NUECS_PIPELINE_H

#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/system_manager.h>

typedef struct {
    nu_string_t name;
    nu_array_t systems;  /* nuecs_system_t */
    nu_array_t sequence; /* uint32_t */
    uint32_t id;
} nuecs_pipeline_data_t;

typedef struct {
    void *state;
    nuecs_system_start_pfn_t start;
    nuecs_system_stop_pfn_t stop;
    nuecs_system_update_pfn_t update;
} nuecs_system_instance_t;

typedef struct {
    nuecs_pipeline_t pipeline;
    nu_array_t system_instances; /* nuecs_pipeline_instance_system_t */
} nuecs_pipeline_instance_data_t;

nu_result_t nuecs_pipeline_initialize(nuecs_pipeline_data_t *pipeline, const nuecs_pipeline_info_t *info);
nu_result_t nuecs_pipeline_terminate(nuecs_pipeline_data_t *pipeline);

nu_result_t nuecs_pipeline_instance_initialize(
    nuecs_pipeline_instance_data_t *instance,
    nuecs_system_manager_data_t *manager,
    nuecs_pipeline_t pipeline
);
nu_result_t nuecs_pipeline_instance_terminate(nuecs_pipeline_instance_data_t *instance);
nu_result_t nuecs_pipeline_instance_start(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);
nu_result_t nuecs_pipeline_instance_stop(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);
nu_result_t nuecs_pipeline_instance_update(
    nuecs_pipeline_instance_data_t *instance,
    nuecs_system_manager_data_t *manager,
    nuecs_scene_t scene
);

#endif