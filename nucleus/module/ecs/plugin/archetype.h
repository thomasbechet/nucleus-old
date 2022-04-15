#ifndef NUECS_ARCHETYPE_H
#define NUECS_ARCHETYPE_H

#include <nucleus/module/ecs/plugin/component.h>

typedef struct {
    nu_array_t edges;
    uint32_t component_count;
    uint32_t *data_sizes;
    uint32_t *component_ids;
    bool *is_system_state;
    bool has_system_state;
    uint32_t index;
} nuecs_archetype_data_t;

typedef struct {
    nuecs_archetype_data_t *add;
    nuecs_archetype_data_t *remove;
    uint32_t component_id;
} nuecs_archetype_edge_t;

nu_result_t nuecs_archetype_create_empty(nuecs_archetype_data_t **archetype);
nu_result_t nuecs_archetype_create_next(
    nuecs_archetype_data_t *current_archetype,
    nuecs_component_data_t *component,
    uint32_t component_id,
    nuecs_archetype_data_t **archetype
);
nu_result_t nuecs_archetype_destroy(nuecs_archetype_data_t *archetype);

nu_result_t nuecs_archetype_find_next(
    nuecs_archetype_data_t *current,
    uint32_t component_id,
    nuecs_archetype_data_t **next
);
nu_result_t nuecs_archetype_find_previous(
    nuecs_archetype_data_t *current,
    uint32_t component_id,
    nuecs_archetype_data_t **previous
);
nu_result_t nuecs_archetype_link_if_previous(
    nuecs_archetype_data_t *current,
    nuecs_archetype_data_t *previous
);

nu_result_t nuecs_archetype_find_component_index(
    const nuecs_archetype_data_t *archetype, 
    uint32_t component_id, 
    uint32_t *index
);

#endif