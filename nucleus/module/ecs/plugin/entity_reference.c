#include <nucleus/module/ecs/plugin/entity_reference.h>

#include <nucleus/module/ecs/plugin/utility.h>
#include <nucleus/module/ecs/plugin/logger.h>

nu_result_t nuecs_entity_reference_bind(
    nuecs_scene_data_t *scene,
    nuecs_entity_reference_t *handle,
    nuecs_entity_t entity
)
{
    /* check null handle */
    if (entity == NU_NULL_HANDLE) {
        *handle = NU_NULL_HANDLE;
        return NU_FAILURE; /* prefer set the reference to NU_NULL_HANDLE */
    }

    /* get chunk */
    nuecs_chunk_data_t **pchunk;
    nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(entity), &pchunk);
    nuecs_chunk_data_t *chunk = *pchunk;

    /* check version */
    if (!nuecs_chunk_check_entity(chunk, entity)) {
        *handle = NU_NULL_HANDLE;
        return NU_FAILURE; /* tried to bind with an invalid handle */
    }

    /* get reference index */
    uint32_t reference_index;
    nuecs_chunk_get_entity_reference(chunk, entity, &reference_index);

    /* create reference if needed */
    if (reference_index == NUECS_ENTITY_REFERENCE_NONE) {

        /* create reference */
        if (!nu_array_is_empty(scene->free_references)) {
            uint32_t *pdata;
            nu_array_get_last(scene->free_references, &pdata);
            reference_index = *pdata;
            nu_array_pop(scene->free_references);
        } else {
            reference_index = nu_array_get_size(scene->references);
            nuecs_entity_reference_data_t ref = {
                .entity  = entity,
                .version = nuecs_next_version(&scene->next_reference_version)
            };
            nu_array_push(scene->references, &ref);
        }

        /* set reference index */
        nuecs_chunk_set_entity_reference(chunk, entity, reference_index);
    }

    /* get reference */
    nuecs_entity_reference_data_t *reference;
    nu_array_get(scene->references, reference_index, &reference);
    
    /* return handle */
    *handle = NUECS_ENTITY_REFERENCE_BUILD_HANDLE(reference->version, reference_index);

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_reference_resolve(
    nuecs_scene_data_t *scene,
    nuecs_entity_reference_t *handle,
    nuecs_entity_t *entity
)
{
    /* check not null */
    if (*handle == NU_NULL_HANDLE) {

        /* invalid reference */
        *entity = NU_NULL_HANDLE;
        return NU_SUCCESS;
    }

    /* get reference */
    nuecs_entity_reference_data_t *reference;
    nu_array_get(scene->references, NUECS_ENTITY_REFERENCE_INDEX(*handle), &reference);

    /* check version */
    if (reference->version != NUECS_ENTITY_REFERENCE_VERSION(*handle)) {
        
        /* invalid reference, update reference and return NULL */
        *handle = NU_NULL_HANDLE;
        *entity = NU_NULL_HANDLE;
        return NU_SUCCESS;
    }

    /* return entity */
    *entity = reference->entity;

    return NU_SUCCESS;
}
static bool nuecs_entity_equals(const void *a, const void *b) {
    return *((nuecs_entity_t*)a) == *((nuecs_entity_t*)b);
}
nu_result_t nuecs_entity_reference_serialize_json_object(
    nuecs_entity_reference_t *handle,
    nuecs_serialization_context_t context, 
    nu_json_object_t object, 
    const char* name
)
{
    /* get context */
    nuecs_serialization_context_data_t *serialization = (nuecs_serialization_context_data_t*)context;

    /* find entity handle */
    nuecs_entity_t entity;
    nuecs_entity_reference_resolve(serialization->scene, handle, &entity);

    /* chunk null entity */
    if (entity == NU_NULL_HANDLE) {
        return nu_json_object_put_null(object, name);
    }
    
    /* find id and put id */
    uint32_t id;
    if (nu_array_find_index(serialization->entities, nuecs_entity_equals, &entity, &id)) {
        return nu_json_object_put_uint(object, name, id);
    } else {
        return nu_json_object_put_null(object, name);
    }
}
nu_result_t nuecs_entity_reference_deserialize_json_object(
    nuecs_deserialization_context_t context, 
    nu_json_object_t object, 
    const char* name, 
    nuecs_entity_reference_t* handle
)
{
    /* get context */
    nuecs_deserialization_context_data_t *deserialization = (nuecs_deserialization_context_data_t*)context;
    
    /* check null value */
    nu_json_value_t value = nu_json_object_get_by_name(object, name);
    if (nu_json_value_is_null(value)) {
        *handle = NU_NULL_HANDLE;
        return NU_SUCCESS;
    }

    /* get id */
    uint32_t id;
    NU_CHECK(nu_json_value_as_uint(value, &id), return NU_FAILURE, NUECS_LOGGER_NAME,
        "Failed to deserialize entity from object field %s.", name);

    /* check valid id */
    NU_CHECK(id < deserialization->entity_count, return NU_FAILURE, NUECS_LOGGER_NAME, "Invalid entity id.");

    /* bind reference */
    return nuecs_entity_reference_bind(deserialization->scene, handle, deserialization->entities[id]);
}
nu_result_t nuecs_entity_reference_remap(
    nuecs_transfer_context_t context, 
    nuecs_entity_reference_t* handle
)
{


    return NU_SUCCESS;
}