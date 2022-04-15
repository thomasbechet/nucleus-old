#ifndef NUECS_PIPELINE_MANAGER_H
#define NUECS_PIPELINE_MANAGER_H

#include <nucleus/module/ecs/plugin/types_public.h>

typedef struct {
    nu_array_t pipelines;
} nuecs_pipeline_manager_data_t;

nu_result_t nuecs_pipeline_manager_initialize(nuecs_pipeline_manager_data_t *manager);
nu_result_t nuecs_pipeline_manager_terminate(nuecs_pipeline_manager_data_t *manager);

nu_result_t nuecs_pipeline_manager_build_component(
    nuecs_pipeline_manager_data_t *manager,
    const nuecs_pipeline_info_t *info, 
    nuecs_pipeline_t *handle
);

#endif