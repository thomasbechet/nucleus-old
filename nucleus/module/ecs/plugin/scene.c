#include <nucleus/module/ecs/plugin/scene.h>

#include <nucleus/module/ecs/plugin/chunk_table.h>
#include <nucleus/module/ecs/plugin/utility.h>
#include <nucleus/module/ecs/plugin/query.h>
#include <nucleus/module/ecs/plugin/entity.h>
#include <nucleus/module/ecs/plugin/entity_reference.h>
#include <nucleus/module/ecs/plugin/system.h>
#include <nucleus/module/ecs/plugin/logger.h>

nu_result_t nuecs_scene_create(nuecs_scene_manager_data_t *manager, nuecs_scene_t *handle)
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
    nu_array_allocate(&scene->chunks, sizeof(nuecs_chunk_data_t*));
    nuecs_chunk_table_initialize(&scene->chunk_table);
    nu_indexed_array_allocate(&scene->queries, sizeof(nuecs_query_data_t*));
    nu_array_allocate(&scene->references, sizeof(nuecs_entity_reference_data_t));
    nu_array_allocate(&scene->free_references, sizeof(uint32_t));
    scene->next_reference_version = 0;

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

    /* entities */
    nuecs_scene_clear(scene);

    /* free other resources */
    nuecs_chunk_table_terminate(&scene->chunk_table);
    nu_array_free(scene->entities_to_delete);
    nu_array_free(scene->chunks);
    nu_array_free(scene->references);
    nu_array_free(scene->free_references);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_clear(nuecs_scene_data_t *scene)
{
    nuecs_chunk_data_t **chunks; uint32_t chunk_count;
    nu_array_get_data(scene->chunks, &chunks, &chunk_count);
    nu_info("chunk_count", "%d", chunk_count);
    for (uint32_t i = 0; i < chunk_count; i++) {
        nuecs_entity_t handles[NUECS_CHUNK_SIZE];
        uint8_t size = chunks[i]->size;
        for (uint8_t j = 0; j < size; j++) {
            nuecs_chunk_get_entity(chunks[i], j, &handles[j]);
        }
        for (uint8_t j = 0; j < size; j++) {
            nuecs_entity_destroy(scene, handles[j]);
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_set_pipeline(
    nuecs_system_manager_data_t *system_manager,
    nuecs_scene_data_t *scene,
    nuecs_pipeline_t pipeline_handle
)
{
    /* get pipeline */
    uint32_t pipeline_id;
    NU_HANDLE_GET_ID(pipeline_handle, pipeline_id);
    nuecs_pipeline_data_t *pipeline;
    nu_array_get(system_manager->pipelines, pipeline_id, &pipeline);

    /* iterate over systems */
    nuecs_system_t *systems; uint32_t system_count;
    nu_array_get_data(pipeline->systems, &systems, &system_count);
    for (uint32_t i = 0; i < system_count; i++) {
        
        
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_progress(nuecs_scene_data_t *scene)
{
    /* remove entities */
    nuecs_entity_t *entities; uint32_t entity_count;
    nu_array_get_data(scene->entities_to_delete, &entities, &entity_count);
    for (uint32_t i = 0; i < entity_count; i++) {
        nuecs_entity_destroy(scene, entities[i]);
    }
    nu_array_clear(scene->entities_to_delete);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_serialize_json_object(
    nuecs_component_manager_data_t *manager, 
    nuecs_scene_data_t *scene, 
    nu_json_object_t object
)
{
    /* put entities array */
    nu_json_array_t j_entities;
    nu_json_object_put_empty_array(object, "entities", &j_entities);

    /* create serialization context */
    nuecs_serialization_context_data_t serialization;
    serialization.scene = scene;
    nu_array_allocate(&serialization.entities, sizeof(nuecs_entity_t));

    /* get chunks */
    nuecs_chunk_data_t **chunks; uint32_t chunk_count;
    nu_array_get_data(scene->chunks, &chunks, &chunk_count);

    /* compute array of entities */
    for (uint32_t i = 0; i < chunk_count; i++) {
        for (uint8_t j = 0; j < chunks[i]->size; j++) {

            /* get handle */
            nuecs_entity_t handle;
            nuecs_chunk_get_entity(chunks[i], j, &handle);

            /* add to remap */
            nu_array_push(serialization.entities, &handle);
        }
    }

    /* serialize entities */
    nuecs_entity_t *entities; uint32_t entity_count;
    nu_array_get_data(serialization.entities, &entities, &entity_count);
    for (uint32_t i = 0; i < entity_count; i++) {

        /* create entity object */
        nu_json_object_t j_entity;
        nu_json_array_add_empty_object(j_entities, &j_entity);
        nu_json_object_put_uint(j_entity, "id", i);

        /* put components */
        nu_json_object_t j_components;
        nu_json_object_put_empty_object(j_entity, "components", &j_components);

        /* get chunk */
        nuecs_chunk_data_t **chunk;
        nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(entities[i]), &chunk);

        /* iterate over components */
        const nuecs_archetype_data_t *archetype = (*chunk)->archetype;
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            uint32_t id = archetype->component_ids[j];

            /* get component */
            nuecs_component_data_t *component;
            nu_array_get(manager->components, id, &component);

            /* put component data */
            nu_json_object_t j_component;
            nu_json_object_put_empty_object(j_components, nu_string_get_cstr(component->name), &j_component);

            if (component->serialize_json) {

                /* find component data */
                uint32_t index;
                nuecs_archetype_find_component_index(archetype, id, &index);
                nuecs_component_data_ptr_t data;
                nuecs_chunk_get_component(*chunk, entities[i], index, &data);

                /* serialize component */
                component->serialize_json(data, (nuecs_serialization_context_t)&serialization, j_component);
            }
        }

    }

    /* free resources */
    nu_array_free(serialization.entities);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_deserialize_json_object(
    nuecs_component_manager_data_t *manager, 
    nuecs_scene_data_t *scene, 
    nu_json_object_t object
)
{
    nu_result_t result = NU_SUCCESS;

    /* get entities json list */
    nu_json_array_t j_entities;
    result = nu_json_value_as_array(nu_json_object_get_by_name(object, "entities"), &j_entities);
    NU_CHECK(result == NU_SUCCESS, return result, NUECS_LOGGER_NAME, "Scene json doesn't have 'entities' field.");

    /* components placeholder */
    uint32_t components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;

    /* get entity count */
    uint32_t entity_count = nu_json_array_get_size(j_entities);

    /* deserialization context */
    nuecs_deserialization_context_data_t deserialization;
    deserialization.scene        = scene;
    deserialization.entities     = (nuecs_entity_t*)nu_malloc(sizeof(nuecs_entity_t) * entity_count);
    deserialization.entity_count = 0;

    /******************/
    /* CREATE ENTRIES */
    /******************/

    /* iterate over entities */
    nu_json_array_iterator_t j_entity_iterator = NU_JSON_ITERATOR_INITIALIZE;
    while (nu_json_array_next(j_entities, &j_entity_iterator)) {

        /* get entity object */
        nu_json_object_t j_entity;
        result = nu_json_value_as_object(nu_json_array_iterator_get_value(j_entity_iterator), &j_entity);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Json entity is not an object.");

        /* get entity id */
        uint32_t id;
        result = nu_json_value_as_uint(nu_json_object_get_by_name(j_entity, "id"), &id);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Json entity id is not a uint.");

        /* get components field */
        nu_json_object_t j_components;
        result = nu_json_value_as_object(nu_json_object_get_by_name(j_entity, "components"), &j_components);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Json component list is not an object.");

        /* initialize component count */
        component_count = 0;

        /* iterate over components */
        nu_json_object_iterator_t j_components_iterator = NU_JSON_ITERATOR_INITIALIZE;
        while (nu_json_object_next(j_components, &j_components_iterator)) {

            /* find component id */
            uint32_t component_id;
            result = nuecs_component_manager_find_component_by_name(manager,
                nu_json_object_iterator_get_name(j_components_iterator), NULL, &component_id);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to find component %s for entity %d.",
                nu_json_object_iterator_get_name(j_components_iterator), id);
            components[component_count++] = component_id;
        }

        /* add entry */
        nuecs_entity_t handle;
        nuecs_entity_add(manager, scene, components, component_count, &handle);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to create entity.");

        /* save handle */
        deserialization.entities[id] = handle;
        deserialization.entity_count++;
    }

    /**************************/
    /* DESERIALIZE COMPONENTS */
    /**************************/

    /* iterate over entities */
    j_entity_iterator = NU_JSON_ITERATOR_INITIALIZE;
    while (nu_json_array_next(j_entities, &j_entity_iterator)) {

        /* get entity object */
        nu_json_object_t j_entity;
        result = nu_json_value_as_object(nu_json_array_iterator_get_value(j_entity_iterator), &j_entity);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Json entity is not an object.");

        /* get entity */
        uint32_t id;
        result = nu_json_value_as_uint(nu_json_object_get_by_name(j_entity, "id"), &id);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Entity id is not a uint.");

        /* get components field */
        nu_json_object_t j_components;
        result = nu_json_value_as_object(nu_json_object_get_by_name(j_entity, "components"), &j_components);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Component list is not an object.");

        /* get handle and chunk */
        nuecs_entity_t entity = deserialization.entities[id];
        nuecs_chunk_data_t **chunk;
        nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(entity), &chunk);

        /* iterate over components */
        const nuecs_archetype_data_t *archetype = (*chunk)->archetype;
        for (uint32_t component_index = 0; component_index < archetype->component_count; component_index++) {

            /* find component info */
            nuecs_component_data_t *component;
            nu_array_get(manager->components, archetype->component_ids[component_index], &component);

            /* check if the component can be deserialize */
            if (component->deserialize_json) {

                /* get component json */
                nu_json_object_t j_component;
                result = nu_json_value_as_object(nu_json_object_get_by_name(j_components, nu_string_get_cstr(component->name)), &j_component);
                NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Component %s was not found or is not an object.", nu_string_get_cstr(component->name));

                /* get component data */
                nuecs_component_data_ptr_t data;
                nuecs_chunk_get_component(*chunk, entity, component_index, &data);

                /* deserialize */
                result = component->deserialize_json(data, (nuecs_deserialization_context_t)&deserialization, j_component);
                NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to deserialize component %s for entity %d.",
                    nu_string_get_cstr(component->name), id);
            }
        }
    }

    nu_free(deserialization.entities);
    return NU_SUCCESS;

cleanup0:
    /* remove added entry */
    for (uint32_t i = 0; i < deserialization.entity_count; i++) {
        nuecs_entity_remove(scene, deserialization.entities[i]);
    }
    nu_free(deserialization.entities);

    return result;
}
nu_result_t nuecs_scene_save_json(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    const char* filename
)
{
    /* allocate json */
    nu_json_t json; 
    nu_json_object_t root;
    nu_json_allocate_empty_object(&json);
    nu_json_value_as_object(nu_json_get_root(json), &root);

    /* serialize scene */
    nu_result_t result = nuecs_scene_serialize_json_object(manager, scene, root);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to serialize scene.");

    /* save json */
    result = nu_json_save_file(json, filename, false);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to save scene.");

cleanup0:
    nu_json_free(json);

    return result;
}
nu_result_t nuecs_scene_load_json(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    const char* filename
)
{
    /* allocate json */
    nu_json_t json;
    nu_result_t result = nu_json_allocate_from_file(&json, filename);
    NU_CHECK(result == NU_SUCCESS, return result, NUECS_LOGGER_NAME, "Failed load json scene.");
    nu_json_object_t root;
    nu_json_value_as_object(nu_json_get_root(json), &root);

    /* serialize scene */
    result = nuecs_scene_deserialize_json_object(manager, scene, root);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to deserialize scene.");

cleanup0:
    nu_json_free(json);

    return result;
}
nu_result_t nuecs_scene_debug_entities(nuecs_scene_data_t *scene)
{
    // nuecs_entity_data_t *entities;
    // uint32_t entity_count;
    // nu_array_get_data(scene->entities, &entities, &entity_count);
    // nu_info(NUECS_LOGGER_NAME, "|----SCENE----|");
    // nu_info(NUECS_LOGGER_NAME, "|VERSION|INDEX|   UUID   |");
    // for (uint32_t i = 0; i < entity_count; i++) {
    //     if (entities[i].chunk) {
    //         nu_info(NUECS_LOGGER_NAME, "|%07X|%05d|%010u|", entities[i].version, i, NUECS_ENTITY_BUILD_ID(entities[i].version, i));
    //     } else {
    //         nu_info(NUECS_LOGGER_NAME, "|-------|-----|----------|", i);
    //     }
    // }

    return NU_SUCCESS;
}