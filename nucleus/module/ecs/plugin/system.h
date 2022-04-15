#ifndef NUECS_SYSTEM_H
#define NUECS_SYSTEM_H

#include <nucleus/module/ecs/plugin/types_public.h>

typedef struct {
    nu_string_t name;
    nuecs_system_initialize_pfn_t initialize;
    nuecs_system_terminate_pfn_t terminate;
    nuecs_system_invoke_pfn_t invoke;
    uint32_t state_size;
} nuecs_system_data_t;

nu_result_t nuecs_system_initialize(
    nuecs_system_data_t *system,
    const nuecs_system_info_t *info
);
nu_result_t nuecs_system_terminate(
    nuecs_system_data_t *system
);

#endif