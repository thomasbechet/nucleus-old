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
        /* types */
        archetype->types = (nuecs_component_type_t**)nu_malloc(sizeof(nuecs_component_type_t*) * type_count);
        memcpy(archetype->types, types, sizeof(nuecs_component_type_t*) * type_count);
        /* offsets and sizes */
        archetype->data_offsets            = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);
        archetype->data_sizes              = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);
        archetype->chunk_data_total_size   = 0;
        for (uint32_t i = 0; i < type_count; i++) {
            archetype->data_offsets[i] = archetype->chunk_data_total_size;
            archetype->data_sizes[i]   = types[i]->size;
            archetype->chunk_data_total_size     += types[i]->size * NUECS_CHUNK_SIZE; 
        }
    }
    
    nu_array_allocate(&archetype->chunks, sizeof(nuecs_chunk_data_t));
    nu_array_allocate(&archetype->edges, sizeof(nuecs_archetype_edge_t));

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_destroy(nuecs_archetype_t *archetype)
{
    nuecs_chunk_data_t *chunks = (nuecs_chunk_data_t*)nu_array_get_data(archetype->chunks);
    uint32_t chunk_count       = nu_array_get_size(archetype->chunks);
    for (uint32_t i = 0; i < chunk_count; i++) {
        nuecs_chunk_destroy(&chunks[i]);
    }
    nu_array_free(archetype->chunks);
    nu_array_free(archetype->edges);
    if (archetype->type_count > 0) {
        nu_free(archetype->types);
        nu_free(archetype->data_offsets);
        nu_free(archetype->data_sizes);
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_find(nu_array_t archetypes, nuecs_archetype_t *root, nuecs_component_type_t **types, uint32_t type_count, nuecs_archetype_t **find, bool *created)
{
    nuecs_archetype_t *current = root;
    *created = false;

    for (uint32_t i = 0; i < type_count; i++) {
        nuecs_archetype_edge_t *edges = (nuecs_archetype_edge_t*)nu_array_get_data(current->edges);
        uint32_t edge_count = nu_array_get_size(current->edges);

        bool edge_found = false;
        for (uint32_t j = 0; j < edge_count; j++) {
            if (edges[j].type == types[i]->id && edges[j].add) {
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
                        if (types[k]->id != archetypes_data[j]->types[k]->id) {
                            same = false;
                            break;
                        }
                    }
                    if (same) {
                        nuecs_archetype_link(current, archetypes_data[j], types[i]->id);
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

                /* add links */
                nuecs_archetype_link(current, new, types[i]->id);
                current = new;

                *created = true;
            }
        }
    }

    *find = current;
    return NU_SUCCESS;
}
static bool nuecs_find_chunk_not_full(const void *user, const void *object) {
    (void)user;
    nuecs_chunk_data_t *data = (nuecs_chunk_data_t*)object;
    return data->size < NUECS_CHUNK_SIZE;
}
nu_result_t nuecs_archetype_add(nuecs_archetype_t *archetype, nuecs_component_data_ptr_t *data, nuecs_entity_data_t *entity)
{    
    nuecs_chunk_data_t *chunk = NULL;
    uint32_t index;
    if (nu_array_find_index(archetype->chunks, nuecs_find_chunk_not_full, NULL, &index)) {
        chunk = (nuecs_chunk_data_t*)nu_array_get(archetype->chunks, index);
    } else {
        nu_array_push(archetype->chunks, NULL);
        index = nu_array_get_size(archetype->chunks) - 1;
        chunk = (nuecs_chunk_data_t*)nu_array_get_last(archetype->chunks);
        nuecs_chunk_create(chunk, archetype->chunk_data_total_size, NUECS_CHUNK_SIZE);
    }

    nuecs_chunk_add(chunk, archetype->data_offsets, archetype->data_sizes, data, archetype->type_count, &entity->row);
    entity->chunk     = index;
    entity->archetype = archetype;

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_remove(nuecs_archetype_t *archetype, const nuecs_entity_data_t *entity)
{
    return NU_SUCCESS;
}