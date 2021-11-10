#ifndef NUECS_ARCHETYPE_H
#define NUECS_ARCHETYPE_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_archetype_create(nuecs_archetype_t *archetype, nuecs_id_t *types, uint32_t type_count);
nu_result_t nuecs_archetype_destroy(nuecs_archetype_t *archetype);
nu_result_t nuecs_archetype_find(nuecs_archetype_t *root, nuecs_archetype_t **archetype);

#endif