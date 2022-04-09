#ifndef NUECS_SYSTEM_H
#define NUECS_SYSTEM_H

#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/system_manager.h>

typedef struct {
    nu_string_t name;
    // nuecs_system_callback_pfn_t callback;
} nuecs_system_data_t;

typedef struct {
    uint32_t data;
} nuecs_pipeline_command_data_t;

typedef struct {
    nu_array_t systems;
} nuecs_pipeline_data_t;

nu_result_t nuecs_system_initialize(
    nuecs_system_data_t *system,
    const nuecs_system_info_t *info
);
nu_result_t nuecs_system_terminate(
    nuecs_system_data_t *system
);

nu_result_t nuecs_pipeline_initialize(
    nuecs_pipeline_data_t *pipeline,
    const nuecs_pipeline_info_t *info
);
nu_result_t nuecs_pipeline_terminate(
    nuecs_pipeline_data_t *pipeline
);

#endif