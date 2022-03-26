#include <nucleus/module/ecs/plugin/system.h>

// #define CHUNK_VIEW_SIZE(c) (sizeof(nuecs_chunk_view_t) + sizeof(nuecs_component_data_ptr_t) * (c - 1))

// nu_result_t nuecs_system_create(nuecs_system_data_t *system, nuecs_component_data_t **components, uint32_t component_count)
// {
//     nu_array_allocate(&system->chunks, CHUNK_VIEW_SIZE(component_count));
//     system->component_ids = (uint32_t*)nu_malloc(sizeof(uint32_t) * component_count);
//     for (uint32_t i = 0; i < component_count; i++) {
//         system->component_ids[i] = components[i]->id;
//     }
//     system->component_count = component_count;

//     return NU_SUCCESS;
// }
// nu_result_t nuecs_system_destroy(nuecs_system_data_t *system)
// {
//     nu_array_free(system->chunks);
//     nu_free(system->component_ids);

//     return NU_SUCCESS;
// }
// nu_result_t nuecs_system_update(nuecs_system_data_t *system)
// {
//     /* iterate over chunks */
//     uint32_t view_count  = nu_array_get_size(system->chunks);
//     for (uint32_t i = 0; i < view_count; i++) {
//         nuecs_chunk_view_t *view = nu_array_get(system->chunks, i); /* sizeof(view) is unknown */
//         if (view->chunk->frame_size == 0) continue;
//         system->update(view->components, view->chunk->frame_size);
//     }

//     return NU_SUCCESS;
// }