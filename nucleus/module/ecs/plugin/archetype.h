#ifndef NUECS_ARCHETYPE_H
#define NUECS_ARCHETYPE_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_archetype_create(nuecs_archetype_t *archetype, nuecs_component_type_t **types, uint32_t type_count);
nu_result_t nuecs_archetype_destroy(nuecs_archetype_t *archetype);
nu_result_t nuecs_archetype_find(
    nu_array_t archetypes,
    nuecs_archetype_t *root, 
    nuecs_component_type_t **types, 
    uint32_t type_count, 
    nuecs_system_data_t **systems,
    uint32_t system_count,
    nuecs_archetype_t **find
);
nu_result_t nuecs_archetype_add_entity(nuecs_archetype_t *archetype, void **data, nuecs_entity_entry_t *entity);
nu_result_t nuecs_archetype_new_system(nuecs_archetype_t *archetype, nuecs_system_data_t *system);


#endif