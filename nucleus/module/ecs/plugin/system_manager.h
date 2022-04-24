#ifndef NUECS_SYSTEM_MANAGER_H
#define NUECS_SYSTEM_MANAGER_H

#include <nucleus/module/ecs/plugin/types_public.h>

typedef struct {
    nu_array_t systems;
    nu_array_t pipelines;
} nuecs_system_manager_data_t;

nu_result_t nuecs_system_manager_initialize(nuecs_system_manager_data_t *manager);
nu_result_t nuecs_system_manager_terminate(nuecs_system_manager_data_t *manager);

nu_result_t nuecs_system_manager_build_system(
    nuecs_system_manager_data_t *manager,
    const nuecs_system_info_t *info, 
    nuecs_system_t *handle
);
nu_result_t nuecs_system_manager_build_pipeline(
    nuecs_system_manager_data_t *manager,
    const nuecs_pipeline_info_t *info, 
    nuecs_pipeline_t *handle
);

#endif