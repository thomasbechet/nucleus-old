#include <nucleus/module/ecs/plugin/component_manager.h>

#include <nucleus/module/ecs/plugin/logger.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/component.h>

nu_result_t nuecs_component_manager_initialize(nuecs_component_manager_data_t *manager)
{
    /* allocate resources */
    nu_array_allocate(&manager->archetypes, sizeof(nuecs_archetype_data_t*));
    nu_indexed_array_allocate(&manager->components, sizeof(nuecs_component_data_t*));
    manager->next_archetype_index = 0;

    /* create empty archetype */
    nuecs_archetype_create_empty(&manager->empty_archetype);
    nu_array_push(manager->archetypes, &manager->empty_archetype);
    manager->empty_archetype->index = manager->next_archetype_index++;

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_terminate(nuecs_component_manager_data_t *manager)
{
    /* archetypes */
    nuecs_archetype_data_t **archetypes;
    uint32_t archetype_count;
    nu_array_get_data(manager->archetypes, &archetypes, &archetype_count);
    for (uint32_t i = 0; i < archetype_count; i++) {
        nuecs_archetype_destroy(archetypes[i]);
    }
    nu_array_free(manager->archetypes);

    /* components */
    nuecs_component_data_t **components;
    uint32_t type_count;
    nu_indexed_array_get_data(manager->components, &components, &type_count);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_free(components[i]->name);
        nu_free(components[i]);
    }
    nu_indexed_array_free(manager->components);

    return NU_SUCCESS;
}

nu_result_t nuecs_component_manager_record_component(
    nuecs_component_manager_data_t *manager,
    const nuecs_component_info_t* info, 
    nuecs_component_t* handle
)
{
    /* allocate memory */
    nuecs_component_data_t *component = (nuecs_component_data_t*)nu_malloc(sizeof(nuecs_component_data_t));
    nu_string_allocate_cstr(&component->name, info->name);
    component->size             = info->size;
    component->serialize_json   = info->serialize_json;
    component->deserialize_json = info->deserialize_json; 

    /* save the component */
    nu_indexed_array_add(manager->components, &component, &component->id);

    /* set the handle */
    *handle = (nuecs_component_t)component;

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_get_component(
    nuecs_component_manager_data_t *manager, 
    uint32_t component_id, 
    nuecs_component_data_t **component
)
{
    nuecs_component_data_t **pcomponent; nu_indexed_array_get(manager->components, component_id, &pcomponent);
    *component = *pcomponent;
    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_find_archetype(
    nuecs_component_manager_data_t *manager,
    nuecs_component_data_t **components,
    uint32_t component_count,
    nuecs_archetype_data_t **archetype
)
{
    /* iterate over components */
    nuecs_archetype_data_t *current = manager->empty_archetype;
    for (uint32_t i = 0; i < component_count; i++) {
        nuecs_archetype_data_t *next;
        nuecs_component_manager_find_next_archetype(manager, current, components[i], &next);
        current = next;
    }
    *archetype = current;

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_find_next_archetype(
    nuecs_component_manager_data_t *manager,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
)
{
     /* find next archetype */
    if (nuecs_archetype_find_next(current, component, archetype) != NU_SUCCESS) {

        /* create new archetype */
        nuecs_archetype_data_t *new;
        nuecs_archetype_create_next(current, component, &new);
        new->index = manager->next_archetype_index++;

        /* link the new archetype */
        nuecs_archetype_data_t **archetypes_data;
        uint32_t archetype_count;
        nu_array_get_data(manager->archetypes, &archetypes_data, &archetype_count);
        for (uint32_t i = 0; i < archetype_count; i++) {
            /* try to link */
            nuecs_archetype_link_if_previous(new, archetypes_data[i]);
        }

        /* add archetype to the list */
        nu_array_push(manager->archetypes, &new);
        *archetype = new;
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_find_previous_archetype(
    nuecs_component_manager_data_t *manager,
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
)
{
    /* find previous archetype */
    nuecs_archetype_data_t *previous;
    nuecs_archetype_find_previous(current, component, &previous);

    /* not found */
    if (!previous) {

        /* allocate component list for the previous */
        nuecs_component_data_t **components = (nuecs_component_data_t**)nu_malloc(sizeof(nuecs_component_data_t*) * (current->component_count - 1));    

        /* copy components */
        uint32_t j = 0;
        for (uint32_t i = 0; i < current->component_count; i++) {
            nuecs_component_data_t **c; nu_indexed_array_get(manager->components, current->component_ids[i], &c);
            if ((*c) != component) {
                components[j++] = (*c);
            }
        }

        /* find the new archetype */
        nuecs_component_manager_find_archetype(manager, components, current->component_count - 1, archetype);

        /* free resources */
        nu_free(components);
    }

    return NU_SUCCESS;
}

nu_result_t nuecs_component_manager_debug_archetypes(nuecs_component_manager_data_t *manager)
{
    nuecs_archetype_data_t **archetypes;
    uint32_t archetype_count;
    nu_array_get_data(manager->archetypes, &archetypes, &archetype_count);
    
    nu_string_t line;
    nu_string_allocate(&line);
    nu_info(NUECS_LOGGER_NAME, "=====ARCHETYPES=====");
    for (uint32_t i = 0; i < archetype_count; i++) {
        nu_string_clear(&line);
        nu_string_append_format(&line, "ARCHETYPE %p [", archetypes[i]);
        for (uint32_t j = 0; j < archetypes[i]->component_count; j++) {
            nuecs_component_data_t **pcomponent; nu_indexed_array_get(manager->components, archetypes[i]->component_ids[j], &pcomponent); 
            nuecs_component_data_t *component = *pcomponent;
            nu_string_append_format(&line, " %s", nu_string_get_cstr(component->name));
        }
        nu_string_append_format(&line, " ]");
        nu_info(NUECS_LOGGER_NAME, nu_string_get_cstr(line));

        nuecs_archetype_edge_t *edges;
        uint32_t edge_count;
        nu_array_get_data(archetypes[i]->edges, &edges, &edge_count);
        for (uint32_t j = 0; j < edge_count; j++) {
            nu_string_clear(&line);
            nuecs_component_data_t **pcomponent; nu_indexed_array_get(manager->components, edges[j].component_id, &pcomponent);
            nuecs_component_data_t *component = *pcomponent;
            if (edges[j].add) {
                nu_string_append_format(&line, "--> ADD %s : %p", nu_string_get_cstr(component->name), edges[j].add);
            }
            if (edges[j].remove) {
                nu_string_append_format(&line, "--> REMOVE %s : %p", nu_string_get_cstr(component->name), edges[j].remove);
            }
            nu_info(NUECS_LOGGER_NAME, nu_string_get_cstr(line));
        }
    }
    nu_string_free(line);

    return NU_SUCCESS;
}