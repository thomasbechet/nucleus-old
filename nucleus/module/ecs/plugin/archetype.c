#include <nucleus/module/ecs/plugin/archetype.h>

#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/utility.h>

static nu_result_t archetype_link(nuecs_archetype_data_t *a, nuecs_archetype_data_t *b, uint32_t component_id)
{
    nuecs_archetype_edge_t *edges;
    uint32_t edge_count;
    bool found;

    /* link a to b (add) */
    nu_array_get_data(a->edges, &edges, &edge_count);
    found = false;
    
    for (uint32_t i = 0; i < edge_count; i++) {
        if (edges[i].component_id == component_id) {
            edges[i].add = b;
            found = true;
        }
    }

    if (!found) {
        nu_array_push(a->edges, NULL);
        nuecs_archetype_edge_t *edge; nu_array_get_last(a->edges, &edge);
        edge->add          = b;
        edge->remove       = NULL;
        edge->component_id = component_id;
    }

    /* link b to a (remove) */
    nu_array_get_data(b->edges, &edges, &edge_count);
    found = false;
    
    for (uint32_t i = 0; i < edge_count; i++) {
        if (edges[i].component_id == component_id) {
            edges[i].remove = a;
            found = true;
        }
    }

    if (!found) {
        nu_array_push(b->edges, NULL);
        nuecs_archetype_edge_t *edge; nu_array_get_last(b->edges, &edge);
        edge->add          = NULL;
        edge->remove       = a;
        edge->component_id = component_id;
    }

    return NU_SUCCESS;
}

nu_result_t nuecs_archetype_create_empty(nuecs_archetype_data_t **archetype)
{
    nuecs_archetype_data_t *new = (nuecs_archetype_data_t*)nu_malloc(sizeof(nuecs_archetype_data_t));
    new->component_count = 0;
    nu_array_allocate(&new->edges, sizeof(nuecs_archetype_edge_t));
    *archetype = new;

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_create_next(
    nuecs_archetype_data_t *current_archetype,
    nuecs_component_data_t *component,
    uint32_t component_id,
    nuecs_archetype_data_t **archetype
)
{
    /* allocate */
    nuecs_archetype_data_t *new = (nuecs_archetype_data_t*)nu_malloc(sizeof(nuecs_archetype_data_t));
    nu_array_allocate(&new->edges, sizeof(nuecs_archetype_edge_t));

    /* copy info from current archetype */
    new->component_count = current_archetype->component_count + 1;
    new->component_ids   = (uint32_t*)nu_malloc(sizeof(uint32_t) * new->component_count);
    new->data_sizes      = (uint32_t*)nu_malloc(sizeof(uint32_t) * new->component_count);
    for (uint32_t i = 0; i < current_archetype->component_count; i++) {
        new->component_ids[i] = current_archetype->component_ids[i];
        new->data_sizes[i]    = current_archetype->data_sizes[i];
    }
    /* copy info from next component */
    new->component_ids[current_archetype->component_count] = component_id;
    new->data_sizes[current_archetype->component_count]    = component->size;
    
    /* return */
    *archetype = new;

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_destroy(nuecs_archetype_data_t *archetype)
{
    nu_array_free(archetype->edges);
    if (archetype->component_count > 0) {
        nu_free(archetype->component_ids);
        nu_free(archetype->data_sizes);
    }
    nu_free(archetype);

    return NU_SUCCESS;
}

nu_result_t nuecs_archetype_find_next(
    nuecs_archetype_data_t *current_archetype,
    uint32_t component_id,
    nuecs_archetype_data_t **next
)
{
    /* get edges of the current archetype */
    nuecs_archetype_edge_t *edges;
    uint32_t edge_count;
    nu_array_get_data(current_archetype->edges, &edges, &edge_count);

    /* try to find the direct neighbour (fast solution) */
    for (uint32_t j = 0; j < edge_count; j++) {
        if (edges[j].component_id == component_id && edges[j].add) {
            *next = edges[j].add;
            return NU_SUCCESS;
        }
    }

    *next = NULL;
    return NU_FAILURE;
}
nu_result_t nuecs_archetype_find_previous(
    nuecs_archetype_data_t *current,
    uint32_t component_id,
    nuecs_archetype_data_t **previous
)
{
    /* get edges of the current archetype */
    nuecs_archetype_edge_t *edges;
    uint32_t edge_count;
    nu_array_get_data(current->edges, &edges, &edge_count);

    /* try to find the direct neighbour (fast solution) */
    for (uint32_t j = 0; j < edge_count; j++) {
        if (edges[j].component_id == component_id && edges[j].remove) {
            *previous = edges[j].remove;
            return NU_SUCCESS;
        }
    }

    *previous = NULL;
    return NU_FAILURE;
}
nu_result_t nuecs_archetype_link_if_previous(
    nuecs_archetype_data_t *current,
    nuecs_archetype_data_t *previous
)
{
    if (previous->component_count == (current->component_count - 1) && nuecs_is_subset(previous->component_ids, previous->component_count, current->component_ids, current->component_count)) {
        /* find the different component id */
        for (uint32_t j = 0; j < current->component_count; j++) {
            bool found = false;
            for (uint32_t k = 0; k < previous->component_count; k++) {
                if (previous->component_ids[k] == current->component_ids[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                archetype_link(previous, current, current->component_ids[j]);
                return NU_SUCCESS;
            }
        }
    }

    return NU_FAILURE;
}

nu_result_t nuecs_archetype_find_component_index(
    const nuecs_archetype_data_t *archetype, 
    uint32_t component_id, 
    uint32_t *index
)
{
    for (uint32_t i = 0; i < archetype->component_count; i++) {
        if (archetype->component_ids[i] == component_id) {
            *index = i;
            return NU_SUCCESS;
        }
    }
    return NU_FAILURE;
}