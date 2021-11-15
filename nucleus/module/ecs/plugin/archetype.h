#ifndef NUECS_ARCHETYPE_H
#define NUECS_ARCHETYPE_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_archetype_create(nuecs_archetype_t *archetype, nuecs_component_type_t **types, uint32_t type_count);
nu_result_t nuecs_archetype_destroy(nuecs_archetype_t *archetype);
nu_result_t nuecs_archetype_find(nu_array_t archetypes, nuecs_archetype_t *root, nuecs_component_type_t **types, uint32_t type_count, nuecs_archetype_t **find, bool *created);
nu_result_t nuecs_archetype_add(nuecs_archetype_t *archetype, void **data, nuecs_entity_data_t *entity);
nu_result_t nuecs_archetype_remove(nuecs_archetype_t *archetype, const nuecs_entity_data_t *entity);

#endif