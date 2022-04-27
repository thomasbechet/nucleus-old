#ifndef NUECS_ARCHETYPE_H
#define NUECS_ARCHETYPE_H

#include <nucleus/module/ecs/plugin/component_manager.h>

#define NUECS_ARCHETYPE_SLOT_NONE 0xFFFFFFFF

typedef struct nuecs_archetype_data {
    nu_array_t edges; /* nuecs_archetype_edge_t */

    uint32_t component_count;
    uint32_t *data_sizes;
    uint32_t *component_ids;

    bool *is_system_state;
    bool has_system_state;
    
    uint32_t archetype_slot;
} nuecs_archetype_data_t;

typedef struct nuecs_archetype_edge {
    nuecs_archetype_data_t *add;
    nuecs_archetype_data_t *remove;
    uint32_t component_id;
} nuecs_archetype_edge_t;

typedef struct {
    nuecs_archetype_data_t *empty_archetype;
    nu_array_t archetypes;
} nuecs_archetype_tree_t;

nu_result_t nuecs_archetype_tree_initialize(nuecs_archetype_tree_t *tree);
nu_result_t nuecs_archetype_tree_terminate(nuecs_archetype_tree_t *tree);

nu_result_t nuecs_archetype_find_next(
    nuecs_archetype_tree_t *tree,
    nuecs_component_manager_data_t *manager,
    nuecs_archetype_data_t *current,
    uint32_t component_id,
    nuecs_archetype_data_t **next
);
nu_result_t nuecs_archetype_find_previous(
    nuecs_archetype_tree_t *tree,
    nuecs_component_manager_data_t *manager,
    nuecs_archetype_data_t *current,
    uint32_t component_id,
    nuecs_archetype_data_t **previous
);
nu_result_t nuecs_archetype_find(
    nuecs_archetype_tree_t *tree,
    nuecs_component_manager_data_t *manager,
    uint32_t *component_ids,
    uint32_t component_count,
    nuecs_archetype_data_t **archetype  
);

nu_result_t nuecs_archetype_find_component_index(
    const nuecs_archetype_data_t *archetype, 
    uint32_t component_id, 
    uint32_t *index
);

#endif