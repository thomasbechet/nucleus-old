#include <nucleus/module/ecs/plugin/component_manager.h>

#include <nucleus/module/ecs/plugin/logger.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/component.h>

nu_result_t nuecs_component_manager_initialize(nuecs_component_manager_data_t *manager)
{
    nu_array_allocate(&manager->components, sizeof(nuecs_component_data_t));
    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_terminate(nuecs_component_manager_data_t *manager)
{
    nuecs_component_data_t *components; uint32_t component_count;
    nu_array_get_data(manager->components, &components, &component_count);
    for (uint32_t i = 0; i < component_count; i++) {
        nuecs_component_terminate(&components[i]);
    }
    nu_array_free(manager->components);
    return NU_SUCCESS;
}

nu_result_t nuecs_component_manager_build_component(
    nuecs_component_manager_data_t *manager,
    const nuecs_component_info_t *info, 
    nuecs_component_t *handle
)
{
    /* allocate memory */
    nuecs_component_data_t *component;
    nu_array_push(manager->components, NULL);
    nu_array_get_last(manager->components, &component);
    NU_HANDLE_SET_ID(*handle, nu_array_get_size(manager->components) - 1);
    return nuecs_component_initialize(component, info);
}
nu_result_t nuecs_component_manager_find_component(
    nuecs_component_manager_data_t *manager,
    const char *name,
    nuecs_component_t *handle
)
{
    uint32_t component_id;
    nu_result_t result = nuecs_component_manager_find_component_by_name(manager, name, NULL, &component_id);
    if (result == NU_SUCCESS) {
        NU_HANDLE_SET_ID(*handle, component_id);
    }
    return result;
}
nu_result_t nuecs_component_manager_find_component_by_name(
    nuecs_component_manager_data_t *manager,
    const char *name,
    nuecs_component_data_t **component_data,
    uint32_t *component_id
)
{
    /* iterate over components */
    nuecs_component_data_t *components;
    uint32_t component_count;
    nu_array_get_data(manager->components, &components, &component_count);
    for (uint32_t i = 0; i < component_count; i++) {
        if (NU_MATCH(nu_string_get_cstr(components[i].name), name)) {
            if (component_id) *component_id     = i;
            if (component_data) *component_data = &components[i]; 
            return NU_SUCCESS;
        }
    }
    return NU_FAILURE;
}

// nu_result_t nuecs_component_manager_debug_archetypes(nuecs_component_manager_data_t *manager)
// {
//     nuecs_archetype_data_t **archetypes;
//     uint32_t archetype_count;
//     nu_array_get_data(manager->archetypes, &archetypes, &archetype_count);
    
//     nu_string_t line;
//     nu_string_allocate(&line);
//     nu_info(NUECS_LOGGER_NAME, "=====ARCHETYPES=====");
//     for (uint32_t i = 0; i < archetype_count; i++) {
//         nu_string_clear(&line);
//         nu_string_append_format(&line, "ARCHETYPE %p [", archetypes[i]);
//         for (uint32_t j = 0; j < archetypes[i]->component_count; j++) {
//             nuecs_component_data_t *component;
//             nu_array_get(manager->components, archetypes[i]->component_ids[j], &component);
//             nu_string_append_format(&line, " %s", nu_string_get_cstr(component->name));
//         }
//         nu_string_append_format(&line, " ]");
//         nu_info(NUECS_LOGGER_NAME, nu_string_get_cstr(line));

//         nuecs_archetype_edge_t *edges;
//         uint32_t edge_count;
//         nu_array_get_data(archetypes[i]->edges, &edges, &edge_count);
//         for (uint32_t j = 0; j < edge_count; j++) {
//             nu_string_clear(&line);
//             nuecs_component_data_t *component;
//             nu_array_get(manager->components, edges[j].component_id, &component);
//             if (edges[j].add) {
//                 nu_string_append_format(&line, "--> ADD %s : %p", nu_string_get_cstr(component->name), edges[j].add);
//             }
//             if (edges[j].remove) {
//                 nu_string_append_format(&line, "--> REMOVE %s : %p", nu_string_get_cstr(component->name), edges[j].remove);
//             }
//             nu_info(NUECS_LOGGER_NAME, nu_string_get_cstr(line));
//         }
//     }
//     nu_string_free(line);

//     return NU_SUCCESS;
// }