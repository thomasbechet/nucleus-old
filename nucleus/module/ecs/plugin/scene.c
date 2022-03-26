#include <nucleus/module/ecs/plugin/scene.h>

#include <nucleus/module/ecs/plugin/archetype_table.h>
#include <nucleus/module/ecs/plugin/utility.h>
#include <nucleus/module/ecs/plugin/query.h>
#include <nucleus/module/ecs/plugin/entity.h>
#include <nucleus/module/ecs/plugin/logger.h>

nu_result_t nuecs_scene_create(nuecs_scene_manager_data_t *manager, nuecs_scene_t* handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)nu_malloc(sizeof(nuecs_scene_data_t));
    nu_indexed_array_add(manager->scenes, &scene, &scene->id);
    *handle = (nuecs_scene_t)scene;
    return nuecs_scene_initialize(scene);
}
nu_result_t nuecs_scene_destroy(nuecs_scene_manager_data_t *manager, nuecs_scene_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_initialize(nuecs_scene_data_t *scene)
{
    /* allocate resources */
    nu_array_allocate(&scene->entities, sizeof(nuecs_entity_data_t));
    nu_array_allocate(&scene->deleted_indices, sizeof(uint32_t));
    nu_array_allocate(&scene->free_indices, sizeof(uint32_t));
    nuecs_archetype_table_initialize(&scene->archetype_table);
    nu_indexed_array_allocate(&scene->queries, sizeof(nuecs_query_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_terminate(nuecs_scene_data_t *scene)
{
    /* queries */
    nuecs_query_data_t **queries;
    uint32_t query_count;
    nu_indexed_array_get_data(scene->queries, &queries, &query_count);
    for (uint32_t i = 0; i < query_count; i++) {
        nuecs_query_terminate(queries[i]);
        nu_free(queries[i]);
    }
    nu_indexed_array_free(scene->queries);

    /* free other resources */
    nuecs_archetype_table_terminate(scene->archetype_table);
    nu_array_free(scene->deleted_indices);
    nu_array_free(scene->free_indices);
    nu_array_free(scene->entities);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_progress(nuecs_scene_data_t *scene)
{
    /* remove entities */
    nuecs_entity_data_t *entities;
    nu_array_get_data(scene->entities, &entities, NULL);
    uint32_t *indices;
    uint32_t index_count;
    nu_array_get_data(scene->deleted_indices, &indices, &index_count);
    for (uint32_t i = 0; i < index_count; i++) {
        /* remove components */
        uint32_t index = indices[i];
        nuecs_chunk_remove(entities[index].chunk, entities[index].chunk_id);
        /* remove entry */
        nu_array_push(scene->free_indices, &index);
        entities[index].chunk = NULL;
    }
    nu_array_clear(scene->deleted_indices);

    return NU_SUCCESS;
}
// nu_result_t nuecs_scene_register_system(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle)
// {
//     nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

//     /* sort types ids */
//     nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
//     uint32_t component_count;
//     sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

//     /* allocate memory */
//     nuecs_system_data_t *system = (nuecs_system_data_t*)nu_malloc(sizeof(nuecs_system_data_t));
//     nuecs_system_create(system, components, component_count);
//     system->update = info->update;

//     /* add system to archetypes */
//     nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(scene->archetypes);
//     uint32_t archetype_count            = nu_array_get_size(scene->archetypes);
//     for (uint32_t i = 0; i < archetype_count; i++) {
//         nuecs_archetype_new_system(archetypes[i], system);
//     }

//     /* save the system */
//     nu_indexed_array_add(scene->systems, &system, &system->id);

//     /* set handle */
//     *handle = (nuecs_system_t)system;

//     return NU_SUCCESS;
// }
nu_result_t nuecs_scene_save_file(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const char* filename
)
{
    /* allocate json */
    nu_json_t json;
    nu_json_allocate_empty_object(&json);
    nu_json_object_t root;
    nu_json_value_as_object(nu_json_get_root(json), &root);

    /* put entities array */
    nu_json_array_t j_entities;
    nu_json_object_put_empty_array(root, "entities", &j_entities);

    /* iterate over entries */
    nuecs_entity_data_t *entities;
    uint32_t entity_count;
    nu_array_get_data(scene->entities, &entities, &entity_count);
    for (uint32_t i = 0; i < entity_count; i++) {

        /* skip entity if not used */
        if (!entities[i].chunk) continue;

        /* create entity object */
        nu_json_object_t j_entity;
        nu_json_array_add_empty_object(j_entities, &j_entity);

        /* put id */
        nu_json_object_put_uint(j_entity, "id", i);

        /* put components */
        nu_json_object_t j_components;
        nu_json_object_put_empty_object(j_entity, "components", &j_components);

        /* iterate over components */
        const nuecs_archetype_data_t *archetype = entities[i].chunk->archetype;
        for (uint32_t k = 0; k < archetype->component_count; k++) {
            uint32_t id = archetype->component_ids[k];

            /* get component */
            nuecs_component_data_t *component;
            nuecs_component_manager_get_component(manager, id, &component);

            /* put component data */
            nu_json_object_t j_component;
            nu_json_object_put_empty_object(j_components, nu_string_get_cstr(component->name), &j_component);

            if (component->serialize_json) {
                /* find component data */
                uint32_t index;
                nuecs_archetype_find_component_index(archetype, id, &index);
                nuecs_component_data_ptr_t data;
                nuecs_chunk_get_component(entities[i].chunk, entities[i].chunk_id, index, &data);

                /* serialize component */
                component->serialize_json(data, j_component);   
            }
        }
    }

    /* save json */
    nu_result_t result = nu_json_save_file(json, filename, false);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to save scene.");

cleanup0:
    nu_json_free(json);

    return result;
}