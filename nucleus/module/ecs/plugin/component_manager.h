#ifndef NUECS_COMPONENT_MANAGER_H
#define NUECS_COMPONENT_MANAGER_H

#include <nucleus/module/ecs/plugin/archetype.h>

typedef struct {
    nu_array_t archetypes;
    nuecs_archetype_data_t *empty_archetype;
    nu_indexed_array_t components;
    uint32_t next_archetype_index;
} nuecs_component_manager_data_t;

nu_result_t nuecs_component_manager_initialize(nuecs_component_manager_data_t *manager);
nu_result_t nuecs_component_manager_terminate(nuecs_component_manager_data_t *manager);

nu_result_t nuecs_component_manager_register_component(
    nuecs_component_manager_data_t *manager,
    const nuecs_component_info_t* info, 
    nuecs_component_t* handle
);
nu_result_t nuecs_component_manager_get_component(
    nuecs_component_manager_data_t *manager,
    uint32_t component_id, 
    nuecs_component_data_t **component
);
nu_result_t nuecs_component_manager_find_archetype(
    nuecs_component_manager_data_t *manager,
    nuecs_component_data_t **components,
    uint32_t component_count,
    nuecs_archetype_data_t **archetype
);
nu_result_t nuecs_component_manager_find_next_archetype(
    nuecs_component_manager_data_t *manager,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
);
nu_result_t nuecs_component_manager_find_previous_archetype(
    nuecs_component_manager_data_t *manager,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
);

nu_result_t nuecs_component_manager_debug_archetypes(
    nuecs_component_manager_data_t *manager
);

#endif