#include <nucleus/module/ecs/plugin/component_manager.h>

#include <nucleus/module/ecs/plugin/archetype.h>

static nuecs_component_manager_data_t _manager;

nu_result_t nuecs_component_manager_initialize(void)
{
    /* allocate resources */
    nu_array_allocate(_manager.archetypes, sizeof(nuecs_archetype_t*));
    nu_indexed_array_allocate(_manager.components, sizeof(nuecs_component_data_t*));
    _manager.next_archetype_index = 0;

    /* create empty archetype */
    nuecs_archetype_create_empty(&_manager.empty_archetype);
    nu_array_push(_manager.archetypes, &_manager.empty_archetype);
    _manager.empty_archetype->index = _manager.next_archetype_index++;

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_terminate(void)
{
    /* archetypes */
    nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(_manager.archetypes);
    uint32_t archetype_count = nu_array_get_size(_manager.archetypes);
    for (uint32_t i = 0; i < archetype_count; i++) {
        nuecs_archetype_destroy(archetypes[i]);
    }
    nu_array_free(_manager.archetypes);

    /* components */
    nuecs_component_data_t **components = (nuecs_component_data_t**)nu_indexed_array_get_data(_manager.components);
    uint32_t type_count = nu_indexed_array_get_size(_manager.components);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_free(components[i]->name);
        nu_free(components[i]);
    }
    nu_indexed_array_free(_manager.components);

    return NU_SUCCESS;
}

nu_result_t nuecs_component_manager_register_component(const nuecs_component_info_t* info, nuecs_component_t* handle)
{
    /* allocate memory */
    nuecs_component_data_t *component = (nuecs_component_data_t*)nu_malloc(sizeof(nuecs_component_data_t));
    nu_string_allocate_cstr(&component->name, info->name);
    component->size             = info->size;
    component->serialize_json   = info->serialize_json;
    component->deserialize_json = info->deserialize_json; 

    /* save the component */
    nu_indexed_array_add(_manager.components, &component, &component->id);

    /* set the handle */
    *handle = (nuecs_component_t)component;

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_find_archetype(
    nuecs_component_data_t **components,
    uint32_t component_count,
    nuecs_archetype_data_t **archetype
)
{
    /* iterate over components */
    nuecs_archetype_data_t *current = _manager.empty_archetype;
    for (uint32_t i = 0; i < component_count; i++) {
        nuecs_archetype_data_t *next;
        nuecs_component_manager_find_next_archetype(current, components[i], &next);
        current = next;
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_find_next_archetype(
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
)
{
     /* find next archetype */
    nuecs_archetype_find_next(current, component, &current);
    
    /* next archetype not found */
    if (!current) {

        /* create new archetype */
        nuecs_archetype_data_t *new;
        nuecs_archetype_create_next(current, component, &new);
        new->index = _manager.next_archetype_index++;

        /* link the new archetype */
        nuecs_archetype_data_t **archetypes_data = (nuecs_archetype_data_t**)nu_array_get_data(_manager.archetypes);
        uint32_t archetype_count                 = nu_array_get_size(_manager.archetypes);
        for (uint32_t i = 0; i < archetype_count; i++) {
            /* try to link */
            nuecs_archetype_link_if_previous(new, archetypes_data[i]);
        }

        /* add archetype to the list */
        nu_array_push(_manager.archetypes, &new);
        *archetype = current;
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_component_manager_find_previous_archetype(
    nuecs_archetype_data_t *current,
    nuecs_component_data_t *component,
    nuecs_archetype_data_t **archetype
)
{
    /* find previous archetype */
    nuecs_archetype_data_t *previous;
    nuecs_archetype_find_previous(archetype, component, &previous);

    /* not found */
    if (!previous) {

        /* allocate component list for the previous */
        nuecs_component_data_t **components = (nuecs_component_data_t**)nu_malloc(sizeof(nuecs_component_data_t*) * (current->component_count - 1));    

        /* copy components */
        uint32_t j = 0;
        for (uint32_t i = 0; i < current->component_count; i++) {
            nuecs_component_data_t *c = (nuecs_component_data_ptr_t*)nu_indexed_array_get(_manager.components, current->component_ids[i]);
            if (c != component) {
                components[j++] = c;
            }
        }

        /* find the new archetype */
        nuecs_component_manager_find_archetype(components, current->component_count - 1, archetype);

        /* free resources */
        nu_free(components);
    }

    return NU_SUCCESS;
}