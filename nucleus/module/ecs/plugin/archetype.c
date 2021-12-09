#include <nucleus/module/ecs/plugin/archetype.h>

#include <nucleus/module/ecs/plugin/chunk.h>

static nu_result_t nuecs_archetype_link(nuecs_archetype_t *a, nuecs_archetype_t *b, uint32_t type)
{
    nuecs_archetype_edge_t *edges;
    uint32_t edge_count;
    bool found;

    /* link a to b (add) */
    edges = (nuecs_archetype_edge_t*)nu_array_get_data(a->edges);
    edge_count = nu_array_get_size(a->edges);
    found = false;
    
    for (uint32_t i = 0; i < edge_count; i++) {
        if (edges[i].type == type) {
            edges[i].add = b;
        }
    }

    if (!found) {
        nu_array_push(a->edges, NULL);
        nuecs_archetype_edge_t *edge = (nuecs_archetype_edge_t*)nu_array_get_last(a->edges);
        edge->add    = b;
        edge->remove = NULL;
        edge->type   = type;
    }

    /* link b to a (remove) */
    edges = (nuecs_archetype_edge_t*)nu_array_get_data(b->edges);
    edge_count = nu_array_get_size(b->edges);
    found = false;
    
    for (uint32_t i = 0; i < edge_count; i++) {
        if (edges[i].type == type) {
            edges[i].remove = a;
        }
    }

    if (!found) {
        nu_array_push(b->edges, NULL);
        nuecs_archetype_edge_t *edge = (nuecs_archetype_edge_t*)nu_array_get_last(b->edges);
        edge->add    = NULL;
        edge->remove = a;
        edge->type   = type;
    }

    return NU_SUCCESS;
}

nu_result_t nuecs_archetype_create(nuecs_archetype_t *archetype, nuecs_component_type_t **types, uint32_t type_count)
{
    archetype->type_count = type_count;
    if (archetype->type_count > 0) {
        archetype->types           = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);
        archetype->data_sizes      = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);
        for (uint32_t i = 0; i < type_count; i++) {
            archetype->data_sizes[i]    = types[i]->size;
            archetype->types[i]         = types[i]->type_id;
        }
    }
    
    nu_array_allocate(&archetype->chunks,  sizeof(nuecs_chunk_data_t*));
    nu_array_allocate(&archetype->edges,   sizeof(nuecs_archetype_edge_t));
    nu_array_allocate(&archetype->systems, sizeof(nuecs_system_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_destroy(nuecs_archetype_t *archetype)
{
    nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(archetype->chunks);
    uint32_t chunk_count       = nu_array_get_size(archetype->chunks);
    for (uint32_t i = 0; i < chunk_count; i++) {
        nuecs_chunk_free(chunks[i]);
    }
    nu_array_free(archetype->chunks);
    nu_array_free(archetype->edges);
    nu_array_free(archetype->systems);
    if (archetype->type_count > 0) {
        nu_free(archetype->types);
        nu_free(archetype->data_sizes);
    }

    return NU_SUCCESS;
}
static bool system_include_archetype_types(const nuecs_archetype_t *archetype, const nuecs_system_data_t *system)
{
    bool has_all = true;
    for (uint32_t i = 0; i < system->type_count; i++) {
        bool found = false;
        for (uint32_t j = 0; j < archetype->type_count; j++) {
            if (archetype->types[j] == system->types[i]) {
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
nu_result_t nuecs_archetype_find(
    nu_array_t archetypes,
    nuecs_archetype_t *root, 
    nuecs_component_type_t **types, 
    uint32_t type_count, 
    nuecs_system_data_t **systems,
    uint32_t system_count,
    nuecs_archetype_t **find
)
{
    nuecs_archetype_t *current = root;

    for (uint32_t i = 0; i < type_count; i++) {
        nuecs_archetype_edge_t *edges = (nuecs_archetype_edge_t*)nu_array_get_data(current->edges);
        uint32_t edge_count = nu_array_get_size(current->edges);

        bool edge_found = false;
        for (uint32_t j = 0; j < edge_count; j++) {
            if (edges[j].type == types[i]->type_id && edges[j].add) {
                current = edges[j].add;
                edge_found = true;
                break;
            }
        }

        if (!edge_found) {
            /* try to find an existing archetype (brute force) */
            nuecs_archetype_t **archetypes_data = (nuecs_archetype_t**)nu_array_get_data(archetypes);
            uint32_t archetype_count = nu_array_get_size(archetypes);

            bool found = false;
            for (uint32_t j = 0; j < archetype_count; j++) {
                if (archetypes_data[j]->type_count == (i + 1)) {
                    bool same = true;
                    for (uint32_t k = 0; k < (i + 1); k++) {
                        if (types[k]->type_id != archetypes_data[j]->types[k]) {
                            same = false;
                            break;
                        }
                    }
                    if (same) {
                        nuecs_archetype_link(current, archetypes_data[j], types[i]->type_id);
                        current = archetypes_data[j];
                        found = true;
                        break;
                    }        
                }
            }

            if (!found) {
                /* no archetype found, create a new one */
                nuecs_archetype_t *new = (nuecs_archetype_t*)nu_malloc(sizeof(nuecs_archetype_t));
                nu_array_push(archetypes, &new);
                nuecs_archetype_create(new, types, i + 1);

                /* add existing systems to the archetype */
                for (uint32_t i = 0; i < system_count; i++) {
                    if (system_include_archetype_types(new, systems[i])) {
                        nu_array_push(new->systems, &systems[i]);
                    }
                }

                /* add links */
                nuecs_archetype_link(current, new, types[i]->type_id);
                current = new;
            }
        }
    }

    *find = current;
    return NU_SUCCESS;
}
static bool nuecs_find_chunk_not_full(const void *user, const void *object) {
    (void)user;
    nuecs_chunk_data_t *data = *(nuecs_chunk_data_t**)object;
    return data->size < NUECS_CHUNK_SIZE;
}
static nu_result_t nuecs_add_chunk_view(nuecs_archetype_t *archetype, nuecs_system_data_t *system, nuecs_chunk_data_t *chunk)
{
    nu_array_push(system->chunks, NULL);
    nuecs_chunk_view_t *view = (nuecs_chunk_view_t*)nu_array_get_last(system->chunks);

    uint32_t indice = 0;
    uint32_t offset = 0;
    for (uint32_t j = 0; j < archetype->type_count && indice < system->type_count; j++) {
        if (archetype->types[j] == system->types[indice]) {
            view->chunk              = chunk;
            view->components[indice] = chunk->data + offset;
            indice++;
        }
        offset += archetype->data_sizes[j] * NUECS_CHUNK_SIZE;
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_add_entity(nuecs_archetype_t *archetype, nuecs_component_data_ptr_t *data, nuecs_entity_entry_t *entity)
{    
    nuecs_chunk_data_t *chunk = NULL;
    uint32_t chunk_index;
    if (nu_array_find_index(archetype->chunks, nuecs_find_chunk_not_full, NULL, &chunk_index)) {
        chunk = *(nuecs_chunk_data_t**)nu_array_get(archetype->chunks, chunk_index);
    } else {
        nuecs_chunk_allocate(archetype, &chunk);
        nu_array_push(archetype->chunks, &chunk);

        /* add the chunk view to the systems */
        nuecs_system_data_t **systems = (nuecs_system_data_t**)nu_array_get_data(archetype->systems);
        uint32_t system_count         = nu_array_get_size(archetype->systems);
        for (uint32_t i = 0; i < system_count; i++) {
            nuecs_add_chunk_view(archetype, systems[i], chunk);
        }
    }

    uint32_t chunk_id;
    nuecs_chunk_add(chunk, data, &chunk_id);
    entity->chunk    = chunk;
    entity->chunk_id = chunk_id;

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_new_system(nuecs_archetype_t *archetype, nuecs_system_data_t *system)
{
    if (system_include_archetype_types(archetype, system)) {
        /* add system to list */
        nu_array_push(archetype->systems, &system);
        /* add chunks to system TODO: add views first (regroup loops) */
        nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(archetype->chunks);
        uint32_t chunk_count        = nu_array_get_size(archetype->chunks);
        for (uint32_t i = 0; i < chunk_count; i++) {
            nuecs_add_chunk_view(archetype, system, chunks[i]);
        }
    }

    return NU_SUCCESS;
}