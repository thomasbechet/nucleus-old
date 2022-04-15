#ifndef NUECS_PIPELINE_H
#define NUECS_PIPELINE_H

#include <nucleus/module/ecs/plugin/types_public.h>

typedef struct {
    nu_string_t name;
    nu_array_t start_systems;
    nu_array_t load_systems;
    nu_array_t update_systems;
    nu_array_t unload_systems;
    nu_array_t stop_systems;
    uint32_t id;
} nuecs_pipeline_data_t;

typedef struct {
    void *state;
    nuecs_system_invoke_pfn_t invoke;
} nuecs_pipeline_command_t;

typedef struct {
    nuecs_pipeline_t pipeline;
    nuecs_pipeline_command_t *commands;
    uint32_t command_count;
} nuecs_pipeline_instance_data_t;

nu_result_t nuecs_pipeline_initialize(nuecs_pipeline_data_t *pipeline, const nuecs_pipeline_info_t *info);
nu_result_t nuecs_pipeline_terminate(nuecs_pipeline_data_t *pipeline);

nu_result_t nuecs_pipeline_instance_initialize(nuecs_pipeline_instance_data_t *instance, const nuecs_pipeline_data_t *pipeline);
nu_result_t nuecs_pipeline_instance_terminate(nuecs_pipeline_instance_data_t *instance);
nu_result_t nuecs_pipeline_instance_start_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);
nu_result_t nuecs_pipeline_instance_load_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);
nu_result_t nuecs_pipeline_instance_update_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);
nu_result_t nuecs_pipeline_instance_store_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);
nu_result_t nuecs_pipeline_instance_stop_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene);

#endif