#ifndef NUECS_ARCHETYPE_H
#define NUECS_ARCHETYPE_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_archetype_create_empty(nuecs_archetype_data_t *archetype);
nu_result_t nuecs_archetype_destroy(nuecs_archetype_data_t *archetype);
nu_result_t nuecs_archetype_find_next(
    nu_array_t archetypes,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *next_component,
    nuecs_system_data_t **systems,
    uint32_t system_count,
    nuecs_archetype_data_t **next
);
nu_result_t nuecs_archetype_find_previous(
    nu_array_t archetypes,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *previous_component,
    nuecs_archetype_data_t **previous
);
nu_result_t nuecs_archetype_find(
    nu_array_t archetypes,
    nuecs_archetype_data_t *root,
    nuecs_component_data_t **components,
    uint32_t component_count,
    nuecs_system_data_t **systems,
    uint32_t system_count,
    nuecs_archetype_data_t **find
);
nu_result_t nuecs_archetype_find_component_index(
    const nuecs_archetype_data_t *archetype, 
    uint32_t component_id, 
    uint32_t *index
);
nu_result_t nuecs_archetype_add_entity(
    nuecs_archetype_data_t *archetype, 
    const nuecs_component_data_t **components,
    const nuecs_component_data_ptr_t *components_data,
    uint32_t component_count,
    nuecs_entity_entry_t *entity
);
nu_result_t nuecs_archetype_remove_entity(
    nuecs_entity_entry_t *entity
);
nu_result_t nuecs_archetype_new_system(
    nuecs_archetype_data_t *archetype, 
    nuecs_system_data_t *system
);
nu_result_t nuecs_archetype_transfer(
    nuecs_entity_entry_t *entity,
    nuecs_archetype_data_t *dst
);

#endif