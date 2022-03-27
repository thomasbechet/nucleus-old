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
    nu_array_allocate(&scene->entities_to_delete, sizeof(nuecs_entity_t));
    nu_array_allocate(&scene->entities, sizeof(nuecs_entity_data_t));
    nu_array_allocate(&scene->free_indices, sizeof(uint32_t));
    nuecs_archetype_table_initialize(&scene->archetype_table);
    nu_indexed_array_allocate(&scene->queries, sizeof(nuecs_query_data_t*));
    scene->next_version = 0;

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
    nu_array_free(scene->free_indices);
    nu_array_free(scene->entities);
    nu_array_free(scene->entities_to_delete);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_progress(nuecs_scene_data_t *scene)
{
    /* remove entities */
    nuecs_entity_data_t *entities;
    nu_array_get_data(scene->entities, &entities, NULL);
    nuecs_entity_t *handles;
    uint32_t handle_count;
    nu_array_get_data(scene->entities_to_delete, &handles, &handle_count);
    for (uint32_t i = 0; i < handle_count; i++) {
        nuecs_entity_destroy(scene, handles[i]);
    }
    nu_array_clear(scene->entities_to_delete);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_serialize_json(
    nuecs_component_manager_data_t *manager, 
    nuecs_scene_data_t *scene, 
    nu_json_object_t object
)
{
    /* put entities array */
    nu_json_array_t j_entities;
    nu_json_object_put_empty_array(object, "entities", &j_entities);

    /* get entities */
    nuecs_entity_data_t *entities;
    uint32_t entity_count;
    nu_array_get_data(scene->entities, &entities, &entity_count);

    /* get free indices */
    uint32_t *free_indices;
    uint32_t free_index_count;
    nu_array_get_data(scene->free_indices, &free_indices, &free_index_count);

    /* create serialization context */
    nuecs_serialization_context_data_t serialization;
    serialization.scene = scene;
    serialization.remap = (uint32_t*)nu_malloc(sizeof(uint32_t) * entity_count);
    uint32_t index = 0;
    for (uint32_t i = 0; i < entity_count; i++) {
        if (!entities[i].chunk) continue;
        serialization.remap[i] = index++;
    }

    /* iterate over entries */
    for (uint32_t i = 0; i < entity_count; i++) {

        /* skip entity if not used */
        if (!entities[i].chunk) continue;

        /* create entity object */
        nu_json_object_t j_entity;
        nu_json_array_add_empty_object(j_entities, &j_entity);

        /* put id */
        nu_json_object_put_uint(j_entity, "id", serialization.remap[i]);

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
                component->serialize_json(data, (nuecs_serialization_context_t)&serialization, j_component);
            }
        }
    }

    /* free serialization context */
    nu_free(serialization.remap);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_save_json(
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

    /* serialize scene */
    nuecs_scene_serialize_json(manager, scene, root);

    /* save json */
    nu_result_t result = nu_json_save_file(json, filename, false);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to save scene.");

cleanup0:
    nu_json_free(json);

    return result;
}
nu_result_t nuecs_scene_debug_entities(nuecs_scene_data_t *scene)
{
    nuecs_entity_data_t *entities;
    uint32_t entity_count;
    nu_array_get_data(scene->entities, &entities, &entity_count);
    nu_info(NUECS_LOGGER_NAME, "|VERSION|INDEX|");
    for (uint32_t i = 0; i < entity_count; i++) {
        if (entities[i].chunk) {
            nu_info(NUECS_LOGGER_NAME, "|%07X|%05d|", entities[i].version, i);
        } else {
            nu_info(NUECS_LOGGER_NAME, "|-------|-----|", i);
        }
    }

    return NU_SUCCESS;
}