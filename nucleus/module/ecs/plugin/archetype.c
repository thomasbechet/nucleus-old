#include <nucleus/module/ecs/plugin/archetype.h>

nu_result_t nuecs_archetype_create(nuecs_archetype_t *archetype, nuecs_id_t *types, uint32_t type_count)
{
    archetype->types = types;
    archetype->type_count = type_count;
    
    nu_array_allocate(&archetype->chunks, sizeof(nuecs_chunk_data_t));
    nu_array_allocate(&archetype->edges, sizeof(nuecs_archetype_edge_t));

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_destroy(nuecs_archetype_t *archetype)
{
    nu_array_free(archetype->chunks);
    nu_array_free(archetype->edges);
    if (archetype->type_count > 0) {
        nu_free(archetype->types);
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_insert(nuecs_archetype_t *root, nuecs_id_t *types, uint32_t type_count, nuecs_archetype_t **archetype)
{
    nuecs_archetype_t *current = root;

    bool exists = true;
    for (uint32_t i = 0; i < type_count; i++) {
        nuecs_archetype_edge_t *edges = (nuecs_archetype_edge_t*)nu_array_get_data(current->edges);
        uint32_t edge_count = nu_array_get_size(current->edges);

        bool found = false;
        for (uint32_t j = 0; j < edge_count; j++) {
            if (edges[j].type == types[i]) {
                current = edges[j].add;
                found = true;
                break;
            }
        }

        if (!found) {
            exists = false;
            break;
        }
    }

    if (exists) {
        *archetype = current;
    } else {
        *archetype = (nuecs_archetype_t*)nu_malloc(sizeof(nuecs_archetype_t));
        
        /* create new archetype */
        nuecs_archetype_create(*archetype, types, type_count);
        /* setup edges */

    }

    return NU_SUCCESS;
}