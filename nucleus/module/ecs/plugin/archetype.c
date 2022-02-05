#include <nucleus/module/ecs/plugin/archetype.h>

#include <nucleus/module/ecs/plugin/chunk.h>

static nu_result_t archetype_link(nuecs_archetype_data_t *a, nuecs_archetype_data_t *b, uint32_t component_id)
{
    nuecs_archetype_edge_t *edges;
    uint32_t edge_count;
    bool found;

    /* link a to b (add) */
    edges = (nuecs_archetype_edge_t*)nu_array_get_data(a->edges);
    edge_count = nu_array_get_size(a->edges);
    found = false;
    
    for (uint32_t i = 0; i < edge_count; i++) {
        if (edges[i].component_id == component_id) {
            edges[i].add = b;
            found = true;
        }
    }

    if (!found) {
        nu_array_push(a->edges, NULL);
        nuecs_archetype_edge_t *edge = (nuecs_archetype_edge_t*)nu_array_get_last(a->edges);
        edge->add          = b;
        edge->remove       = NULL;
        edge->component_id = component_id;
    }

    /* link b to a (remove) */
    edges = (nuecs_archetype_edge_t*)nu_array_get_data(b->edges);
    edge_count = nu_array_get_size(b->edges);
    found = false;
    
    for (uint32_t i = 0; i < edge_count; i++) {
        if (edges[i].component_id == component_id) {
            edges[i].remove = a;
            found = true;
        }
    }

    if (!found) {
        nu_array_push(b->edges, NULL);
        nuecs_archetype_edge_t *edge = (nuecs_archetype_edge_t*)nu_array_get_last(b->edges);
        edge->add          = NULL;
        edge->remove       = a;
        edge->component_id = component_id;
    }

    return NU_SUCCESS;
}

nu_result_t nuecs_archetype_create_empty(nuecs_archetype_data_t *archetype)
{
    archetype->component_count = 0;
    nu_array_allocate(&archetype->chunks,  sizeof(nuecs_chunk_data_t*));
    nu_array_allocate(&archetype->edges,   sizeof(nuecs_archetype_edge_t));
    nu_array_allocate(&archetype->systems, sizeof(nuecs_system_data_t*));

    return NU_SUCCESS;
}
static nu_result_t nuecs_archetype_create_next(nuecs_archetype_data_t *archetype, nuecs_archetype_data_t *current_archetype, nuecs_component_data_t *next_component)
{
    archetype->component_count = current_archetype->component_count + 1;
    archetype->component_ids   = (uint32_t*)nu_malloc(sizeof(uint32_t) * archetype->component_count);
    archetype->data_sizes      = (uint32_t*)nu_malloc(sizeof(uint32_t) * archetype->component_count);
    /* copy info from current archetype */
    for (uint32_t i = 0; i < current_archetype->component_count; i++) {
        archetype->component_ids[i] = current_archetype->component_ids[i];
        archetype->data_sizes[i]    = current_archetype->data_sizes[i];
    }
    /* copy info from next component */
    archetype->component_ids[current_archetype->component_count] = next_component->component_id;
    archetype->data_sizes[current_archetype->component_count]    = next_component->size;
    
    nu_array_allocate(&archetype->chunks,  sizeof(nuecs_chunk_data_t*));
    nu_array_allocate(&archetype->edges,   sizeof(nuecs_archetype_edge_t));
    nu_array_allocate(&archetype->systems, sizeof(nuecs_system_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_destroy(nuecs_archetype_data_t *archetype)
{
    nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(archetype->chunks);
    uint32_t chunk_count       = nu_array_get_size(archetype->chunks);
    for (uint32_t i = 0; i < chunk_count; i++) {
        nuecs_chunk_free(chunks[i]);
    }
    nu_array_free(archetype->chunks);
    nu_array_free(archetype->edges);
    nu_array_free(archetype->systems);
    if (archetype->component_count > 0) {
        nu_free(archetype->component_ids);
        nu_free(archetype->data_sizes);
    }

    return NU_SUCCESS;
}
static bool is_subset(const uint32_t *a, uint32_t ac, const uint32_t *b, uint32_t bc) {
    bool has_all = true;
    for (uint32_t i = 0; i < ac; i++) {
        bool found = false;
        for (uint32_t j = 0; j < bc; j++) {
            if (a[i] == b[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            has_all = false;
            break;
        }
    }
    return has_all;
}
nu_result_t nuecs_archetype_find_next(
    nu_array_t archetypes,
    nuecs_archetype_data_t *current_archetype,
    nuecs_component_data_t *next_component,
    nuecs_system_data_t **systems,
    uint32_t system_count,
    nuecs_archetype_data_t **next
)
{
    /* get edges of the current archetype */
    nuecs_archetype_edge_t *edges = (nuecs_archetype_edge_t*)nu_array_get_data(current_archetype->edges);
    uint32_t edge_count = nu_array_get_size(current_archetype->edges);

    /* try to find the direct neighbour (fast solution) */
    for (uint32_t j = 0; j < edge_count; j++) {
        if (edges[j].component_id == next_component->component_id && edges[j].add) {
            *next = edges[j].add;
            return NU_SUCCESS;
        }
    }

    /* the easy solution was not found, a new archetype must be created */
    nuecs_archetype_data_t *new = (nuecs_archetype_data_t*)nu_malloc(sizeof(nuecs_archetype_data_t));
    nu_array_push(archetypes, &new);
    nuecs_archetype_create_next(new, current_archetype, next_component);

    /* add links (brute force) TODO: optimize me ! */
    nuecs_archetype_data_t **archetypes_data = (nuecs_archetype_data_t**)nu_array_get_data(archetypes);
    uint32_t archetype_count = nu_array_get_size(archetypes);
    for (uint32_t i = 0; i < archetype_count; i++) {
        nuecs_archetype_data_t *a = archetypes_data[i];
        /* check component ids TODO: optimize me ! */
        if (is_subset(a->component_ids, a->component_count, new->component_ids, new->component_count)) {
            /* find the different component id */
            for (uint32_t j = 0; j < new->component_count; j++) {
                bool found = false;
                for (uint32_t k = 0; k < a->component_count; k++) {
                    if (a->component_ids[k] == new->component_ids[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    archetype_link(a, new, new->component_ids[j]);
                    break;
                }
            }
        }
    }

    /* add existing systems to the new archetype */
    for (uint32_t i = 0; i < system_count; i++) {
        nuecs_archetype_new_system(new, systems[i]);
    }

    /* next is the new archetype */
    *next = new;

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_find_previous(
    nu_array_t archetypes,
    nuecs_archetype_data_t *root,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *previous_component,
    nuecs_archetype_data_t **previous
)
{
    /* get edges of the current archetype */
    nuecs_archetype_edge_t *edges = (nuecs_archetype_edge_t*)nu_array_get_data(current->edges);
    uint32_t edge_count           = nu_array_get_size(current->edges);
    /* try to find the direct neighbour (fast solution) */
    for (uint32_t j = 0; j < edge_count; j++) {
        if (edges[j].component_id == previous_component->component_id && edges[j].remove) {
            *previous = edges[j].remove;
            return NU_SUCCESS;
        }
    }

    *previous = NULL;
    return NU_FAILURE;
}
nu_result_t nuecs_archetype_find(
    nu_array_t archetypes,
    nuecs_archetype_data_t *root,
    nuecs_component_data_t **components,
    uint32_t component_count,
    nuecs_system_data_t **systems,
    uint32_t system_count,
    nuecs_archetype_data_t **find
)
{
    nuecs_archetype_data_t *current = root;
    for (uint32_t i = 0; i < component_count; i++) {
        nuecs_archetype_find_next(archetypes, current, components[i], systems, system_count, &current);
    }
    *find = current;

    return NU_SUCCESS;
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
static bool find_chunk_not_full(const void *user, const void *object) {
    (void)user;
    nuecs_chunk_data_t *data = *(nuecs_chunk_data_t**)object;
    return data->size < NUECS_CHUNK_SIZE;
}
static nu_result_t add_chunk_view(nuecs_archetype_data_t *archetype, nuecs_system_data_t *system, nuecs_chunk_data_t *chunk)
{
    nu_array_push(system->chunks, NULL);
    nuecs_chunk_view_t *view = (nuecs_chunk_view_t*)nu_array_get_last(system->chunks);

    view->chunk = chunk;
    for (uint32_t i = 0; i < system->component_count; i++) {
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            if (system->component_ids[i] == archetype->component_ids[j]) {
                view->components[i] = chunk->component_list_ptrs[j];
                break;
            }
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_add_entity(
    nuecs_archetype_data_t *archetype,
    const nuecs_component_data_t **components,
    const nuecs_component_data_ptr_t *components_data,
    uint32_t component_count,
    nuecs_entity_entry_t *entity
)
{    
    /* find free chunk */
    nuecs_chunk_data_t *chunk = NULL;
    uint32_t chunk_index;
    if (nu_array_find_index(archetype->chunks, find_chunk_not_full, NULL, &chunk_index)) {
        chunk = *(nuecs_chunk_data_t**)nu_array_get(archetype->chunks, chunk_index);
    } else {
        /* create new chunk */
        nuecs_chunk_allocate(archetype, &chunk);
        nu_array_push(archetype->chunks, &chunk);
        /* add the chunk view to the systems */
        nuecs_system_data_t **systems = (nuecs_system_data_t**)nu_array_get_data(archetype->systems);
        uint32_t system_count         = nu_array_get_size(archetype->systems);
        for (uint32_t i = 0; i < system_count; i++) {
            add_chunk_view(archetype, systems[i], chunk);
        }
    }

    /* create entity */
    uint32_t chunk_id;
    nuecs_chunk_add(chunk, &chunk_id);
    entity->chunk    = chunk;
    entity->chunk_id = chunk_id;

    /* copy components */
    for (uint32_t i = 0; i < component_count; i++) {
        /* find component index */
        uint32_t component_index;
        nuecs_archetype_find_component_index(archetype, components[i]->component_id, &component_index);
        /* write component */
        nuecs_chunk_write_component(chunk, chunk_id, component_index, components_data[i]);
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_remove_entity(
    nuecs_entity_entry_t *entity
)
{
    nuecs_chunk_remove(entity->chunk, entity->chunk_id);

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_new_system(
    nuecs_archetype_data_t *archetype, 
    nuecs_system_data_t *system
)
{
    if (is_subset(system->component_ids, system->component_count, archetype->component_ids, archetype->component_count)) {
        /* add system to list */
        nu_array_push(archetype->systems, &system);
        /* add chunks to system TODO: add views first (regroup loops) */
        nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(archetype->chunks);
        uint32_t chunk_count        = nu_array_get_size(archetype->chunks);
        for (uint32_t i = 0; i < chunk_count; i++) {
            add_chunk_view(archetype, system, chunks[i]);
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_transfer(
    nuecs_entity_entry_t *entity,
    nuecs_archetype_data_t *dst
)
{
    /* create new entity */
    nuecs_entity_entry_t entry;
    nuecs_archetype_add_entity(dst, NULL, NULL, 0, &entry);

    /* copy data */
    for (uint32_t i = 0; i < dst->component_count; i++) {
        /* find source component index */
        uint32_t src_index; 
        if (nuecs_archetype_find_component_index(entity->chunk->archetype, dst->component_ids[i], &src_index) != NU_FAILURE) {
            /* find source component data */
            nuecs_component_data_ptr_t ptr;
            nuecs_chunk_get_component(entity->chunk, entity->chunk_id, src_index, &ptr);
            /* write component data */
            nuecs_chunk_write_component(entry.chunk, entry.chunk_id, i, ptr);
        }
    }
    
    /* remove entity */
    nuecs_archetype_remove_entity(entity);

    /* write new entry */
    entity->chunk    = entry.chunk;
    entity->chunk_id = entry.chunk_id;

    return NU_SUCCESS;
}