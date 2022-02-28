#ifndef NUECS_COMPONENT_MANAGER_H
#define NUECS_COMPONENT_MANAGER_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_component_manager_initialize(void);
nu_result_t nuecs_component_manager_terminate(void);

nu_result_t nuecs_component_manager_register_component(const nuecs_component_info_t* info, nuecs_component_t* handle);
nu_result_t nuecs_component_manager_find_archetype(
    nuecs_component_data_t **components,
    uint32_t component_count,
    nuecs_archetype_data_t **archetype
);
nu_result_t nuecs_component_manager_find_next_archetype(
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
);
nu_result_t nuecs_component_manager_find_previous_archetype(
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
);

#endif