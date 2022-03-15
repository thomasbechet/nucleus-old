#include <nucleus/module/vulkan/scene/scene.h>

static nu_result_t nuvk_sdf_pool_destroy(
    nuvk_sdf_pool_t *pool
)
{
    nuvk_sdf_instance_data_t *instances;
    uint32_t instance_count;
    nu_indexed_array_get_data(pool->instances, &instances, &instance_count);
    for (uint32_t i = 0; i < instance_count; i++) {
        nu_free(instances[i].data);
    }
    nu_indexed_array_free(pool->instances);
    nu_array_free(pool->instance_free_buffer_indices);
    nu_free(pool->indices);

    return NU_SUCCESS;
}
static nu_result_t nuvk_sdf_pool_generate_indices(nuvk_sdf_pool_t *pool)
{
    nuvk_sdf_instance_data_t *instances;
    uint32_t instance_count;
    nu_indexed_array_get_data(pool->instances, &instances, &instance_count);
    for (uint32_t i = 0; i < instance_count; i++) {
        pool->indices[i] = instances[i].buffer_index;
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_scene_initialize(nuvk_scene_t *scene)
{
    nuvk_camera_initialize(&scene->camera);
    nu_indexed_array_allocate(&scene->instance_handles, sizeof(nuvk_sdf_instance_handle_data_t));
    nu_indexed_array_allocate(&scene->sdf_pools, sizeof(nuvk_sdf_pool_t));

    return NU_SUCCESS;
}
nu_result_t nuvk_scene_terminate(nuvk_scene_t *scene)
{
    nuvk_sdf_pool_t *pools;
    uint32_t pool_count;
    nu_indexed_array_get_data(scene->sdf_pools, &pools, &pool_count);
    for (uint32_t i = 0; i < pool_count; i++) {
        nuvk_sdf_pool_destroy(&pools[i]);
    }

    nu_indexed_array_free(scene->instance_handles);
    nu_indexed_array_free(scene->sdf_pools);

    return NU_SUCCESS;
}

nu_result_t nuvk_scene_sdf_pool_create(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    nuvk_sdf_data_t *sdf_data
)
{
    nu_indexed_array_add(scene->sdf_pools, NULL, &sdf_data->pool_id);
    nuvk_sdf_pool_t *pool = nu_indexed_array_get(scene->sdf_pools, sdf_data->pool_id);

    pool->sdf_data = sdf_data;
    nu_indexed_array_allocate(&pool->instances, sizeof(nuvk_sdf_instance_data_t)); 
    nu_array_allocate(&pool->instance_free_buffer_indices, sizeof(uint32_t));
    pool->indices = (uint32_t*)nu_malloc(sizeof(uint32_t) * sdf_data->max_instance_count);

    nuvk_buffer_instances_write_indices(buffer, active_inflight_frame_index, sdf_data->buffer_id, NULL, 0);

    return NU_SUCCESS;
}

nu_result_t nuvk_scene_sdf_instance_create(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    const nuvk_sdf_instance_info_t *info, 
    nuvk_sdf_instance_t *handle
)
{
    nuvk_sdf_data_t *sdf_data = (nuvk_sdf_data_t*)info->sdf;
    nuvk_sdf_pool_t *pool     = (nuvk_sdf_pool_t*)nu_indexed_array_get(scene->sdf_pools, sdf_data->pool_id);

    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)handle;
    handle_data->pool_id = sdf_data->pool_id; 

    nu_indexed_array_add(pool->instances, NULL, &handle_data->instance_id);
    nuvk_sdf_instance_data_t *instance = (nuvk_sdf_instance_data_t*)nu_indexed_array_get(pool->instances, handle_data->instance_id);
    
    /* data */
    instance->data = nu_malloc(sdf_data->data_size);
    memcpy(instance->data, info->data, sdf_data->data_size);
    /* transform */
    memcpy(&instance->transform, &info->transform, sizeof(nuvk_sdf_transform_t));
    nu_quatf_to_mat3(info->transform.rotation, instance->inv_rotation);
    nu_mat3f_transpose(instance->inv_rotation);
    nu_vec3f_copy(info->transform.translation, instance->translation_scale);
    instance->translation_scale[3] = info->transform.scale;
    /* material */
    instance->material = (nuvk_material_data_t*)info->material;

    /* set buffer id */
    if (!nu_array_is_empty(pool->instance_free_buffer_indices)) {
        uint32_t *free_id = (uint32_t*)nu_array_get_last(pool->instance_free_buffer_indices);
        instance->buffer_index = *free_id;
        nu_array_pop(pool->instance_free_buffer_indices);
    } else {
        instance->buffer_index = nu_indexed_array_get_size(pool->instances) - 1;
    }

    /* regenerate indices */
    nuvk_sdf_pool_generate_indices(pool);

    /* buffer */
    nuvk_buffer_instances_write_indices(buffer, active_inflight_frame_index,
        sdf_data->buffer_id, pool->indices, nu_indexed_array_get_size(pool->instances));
    nuvk_buffer_instances_write_instance_transform(buffer, active_inflight_frame_index,
        pool->sdf_data->buffer_id, instance->buffer_index, instance->inv_rotation, instance->translation_scale);
    nuvk_buffer_instances_write_instance_data(buffer, active_inflight_frame_index,
        pool->sdf_data->buffer_id, instance->buffer_index, instance->data);
    nuvk_buffer_instances_write_instance_material(buffer, active_inflight_frame_index,
        pool->sdf_data->buffer_id, instance->buffer_index, instance->material->buffer_id);

    return NU_SUCCESS;
}
nu_result_t nuvk_scene_sdf_instance_destroy(
    nuvk_scene_t *scene,
    nuvk_sdf_instance_t handle
)
{
    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)handle;
    nuvk_sdf_pool_t *pool = (nuvk_sdf_pool_t*)nu_indexed_array_get(scene->sdf_pools, handle_data->pool_id);
    nuvk_sdf_instance_data_t *instance = (nuvk_sdf_instance_data_t*)nu_indexed_array_get(pool->instances, handle_data->instance_id);
    
    nu_free(instance->data);
    nu_indexed_array_remove(pool->instances, handle_data->instance_id);

    return NU_SUCCESS;
}
nu_result_t nuvk_scene_sdf_instance_update_transform(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    nuvk_sdf_instance_t handle,
    const nuvk_sdf_transform_t *transform
)
{
    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)handle;
    nuvk_sdf_pool_t *pool = (nuvk_sdf_pool_t*)nu_indexed_array_get(scene->sdf_pools, handle_data->pool_id);
    nuvk_sdf_instance_data_t *instance = (nuvk_sdf_instance_data_t*)nu_indexed_array_get(pool->instances, handle_data->instance_id);

    /* transform */
    memcpy(&instance->transform, transform, sizeof(nuvk_sdf_transform_t));
    nu_quatf_to_mat3(instance->transform.rotation, instance->inv_rotation);
    nu_mat3f_transpose(instance->inv_rotation);
    nu_vec3f_copy(instance->transform.translation, instance->translation_scale);
    instance->translation_scale[3] = instance->transform.scale;

    /* buffer */
    nuvk_buffer_instances_write_instance_transform(buffer, active_inflight_frame_index,
        pool->sdf_data->buffer_id, instance->buffer_index, instance->inv_rotation, instance->translation_scale);

    return NU_SUCCESS;
}
nu_result_t nuvk_scene_sdf_instance_update_data(
    nuvk_scene_t *scene,
    nuvk_buffer_instances_t *buffer,
    uint32_t active_inflight_frame_index,
    nuvk_sdf_instance_t handle,
    const void *data
)
{
    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)handle;
    nuvk_sdf_pool_t *pool = (nuvk_sdf_pool_t*)nu_indexed_array_get(scene->sdf_pools, handle_data->pool_id);
    nuvk_sdf_instance_data_t *instance = (nuvk_sdf_instance_data_t*)nu_indexed_array_get(pool->instances, handle_data->instance_id);

    /* data */
    memcpy(instance->data, data, pool->sdf_data->data_size);

    /* buffer */
    nuvk_buffer_instances_write_instance_data(buffer, active_inflight_frame_index,
        pool->sdf_data->buffer_id, instance->buffer_index, instance->data);

    return NU_SUCCESS;
}

// nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene)
// {
//     nuvk_sdf_camera_initialize(&scene->camera);
//     nu_indexed_array_allocate(&scene->instance_handles, sizeof(nuvk_sdf_instance_handle_data_t));
//     scene->type_count     = 0;
//     scene->material_count = 0;
//     nu_array_allocate(&scene->free_material_indices, sizeof(uint32_t));

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene)
// {
//     for (uint32_t t = 0; t < scene->type_count; t++) {
//         nu_free(scene->types[t].instances);
//         nu_free(scene->types[t].data);
//         nu_array_free(scene->types[t].free_instance_indices);
//         nu_free(scene->types[t].indices);
//     }

//     nu_indexed_array_free(scene->instance_handles);
//     nu_array_free(scene->free_material_indices);

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_update_buffers(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_environment_t *environment_buffer,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_buffer_materials_t *materials_buffer
// )
// {
//     /* update buffers */
//     nuvk_sdf_buffer_environment_write_camera(environment_buffer, &scene->camera,
//         render_context->active_inflight_frame_index);

//     /* transfer buffers */
//     nuvk_dynamic_range_buffer_transfer(&environment_buffer->dynamic_range_buffer, render_context->active_inflight_frame_index);
//     nuvk_dynamic_range_buffer_transfer(&instances_buffer->index_buffer, render_context->active_inflight_frame_index);
//     nuvk_dynamic_range_buffer_transfer(&instances_buffer->instance_buffer, render_context->active_inflight_frame_index);
//     nuvk_dynamic_range_buffer_transfer(&materials_buffer->dynamic_range_buffer, render_context->active_inflight_frame_index);

//     return NU_SUCCESS;
// }

// nu_result_t nuvk_sdf_scene_create_material(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_materials_t *material_buffer,
//     const nuvk_sdf_material_info_t *info,
//     nuvk_sdf_material_t *handle
// )
// {
//     /* recover material index */
//     uint32_t material_index;
//     if (!nu_array_is_empty(scene->free_material_indices)) {
//         material_index = *(uint32_t*)nu_array_get_last(scene->free_material_indices);
//         nu_array_pop(scene->free_material_indices);
//     } else {
//         material_index = scene->material_count++;
//     }

//     /* update buffer */
//     scene->materials[material_index] = *info;
//     nuvk_sdf_buffer_materials_write_material(material_buffer, render_context->active_inflight_frame_index,
//         material_index, info);

//     /* save handle */
//     NU_HANDLE_SET_ID(*handle, material_index);

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_destroy_material(
//     nuvk_sdf_scene_t *scene,
//     nuvk_sdf_material_t handle
// )
// {
//     nu_warning(NUVK_LOGGER_NAME, "Destroying material has not been implemented yet.");

//     return NU_SUCCESS;
// }

// nu_result_t nuvk_sdf_scene_register_instance_type(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_sdf_instance_type_info_t *info,
//     nuvk_sdf_instance_type_t *handle
// )
// {
//     NU_CHECK(scene->type_count < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT, return NU_FAILURE,
//         NUVK_LOGGER_NAME, "Max sdf type count reached.");

//     NU_HANDLE_SET_ID(*handle, scene->type_count);
//     nuvk_sdf_instance_type_data_t *type = &scene->types[scene->type_count++];

//     /* setup and allocate memory */
//     type->info = *info;
    
//     type->instances      = (nuvk_sdf_instance_data_t*)nu_malloc(sizeof(nuvk_sdf_instance_data_t) * info->max_instance_count);
//     type->data           = nu_malloc(info->data_size * info->max_instance_count);
//     type->instance_count = 0;
//     nu_array_allocate_capacity(&type->free_instance_indices, sizeof(uint32_t), info->max_instance_count);

//     type->indices     = (uint32_t*)nu_malloc(sizeof(uint32_t) * info->max_instance_count);
//     type->index_count = 0;

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_create_instance(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     const nuvk_sdf_instance_info_t *info,
//     nuvk_sdf_instance_t *handle
// )
// {
//     uint32_t type_index;
//     NU_HANDLE_GET_ID(info->type, type_index);
//     NU_ASSERT(type_index < scene->type_count);
//     nuvk_sdf_instance_type_data_t *type = &scene->types[type_index];
    
//     NU_ASSERT(type->instance_count <= type->info.max_instance_count);

//     /* recover instance index */
//     uint32_t instance_index;
//     if (!nu_array_is_empty(type->free_instance_indices)) {
//         instance_index = *(uint32_t*)nu_array_get_last(type->free_instance_indices);
//         nu_array_pop(type->free_instance_indices);
//     } else {
//         instance_index = type->instance_count++;
//     }

//     /* setup instance */
//     nuvk_sdf_instance_data_t *instance = &type->instances[instance_index];
//     instance->transform = info->transform;
    
//     nu_quatf_to_mat3(info->transform.rotation, instance->inv_rotation);
//     nu_mat3f_transpose(instance->inv_rotation);
//     nu_vec3f_copy(info->transform.translation, instance->translation_scale);
//     instance->translation_scale[3] = info->transform.scale;

//     /* get material index */
//     uint32_t material_index;
//     NU_HANDLE_GET_ID(info->material, material_index);
//     instance->material_index = material_index;

//     /* update indices */
//     instance->index_position = type->index_count;
//     type->indices[instance->index_position] = instance_index;
//     type->index_count++;

//     /* save data */
//     void *pdata = (char*)type->data + instance->index_position * type->info.data_size;
//     memcpy(pdata, info->data, type->info.data_size);

//     /* update buffers */
//     nuvk_sdf_buffer_instances_write_indices(instances_buffer, render_context->active_inflight_frame_index,
//         type_index, type->indices, type->index_count);
//     nuvk_sdf_buffer_instances_write_instance_transform(instances_buffer, render_context->active_inflight_frame_index,
//         type_index, instance_index, instance->inv_rotation, instance->translation_scale);
//     nuvk_sdf_buffer_instances_write_instance_data(instances_buffer, render_context->active_inflight_frame_index,
//         type_index, instance_index, info->data);
//     nuvk_sdf_buffer_instances_write_instance_material(instances_buffer, render_context->active_inflight_frame_index,
//         type_index, instance_index, instance->material_index);

//     /* setup handle */
//     nuvk_sdf_instance_handle_data_t handle_data;
//     handle_data.type_index     = type_index;
//     handle_data.instance_index = instance_index;
//     uint32_t id;
//     nu_indexed_array_add(scene->instance_handles, &handle_data, &id);
//     NU_HANDLE_SET_ID(*handle, id);

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_destroy_instance(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle
// )
// {
//     uint32_t handle_id;
//     NU_HANDLE_GET_ID(handle, handle_id);
//     nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)nu_indexed_array_get(scene->instance_handles, handle_id);
//     (void)handle_data;

//     nu_warning(NUVK_LOGGER_NAME, "Destroying instance has not been implemented yet.");

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_update_instance_transform(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle,
//     const nuvk_sdf_transform_t *transform
// )
// {
//     uint32_t handle_id;
//     NU_HANDLE_GET_ID(handle, handle_id);
//     nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)nu_indexed_array_get(scene->instance_handles, handle_id);
//     NU_ASSERT(handle_data->type_index < scene->type_count);
//     nuvk_sdf_instance_type_data_t *type = &scene->types[handle_data->type_index];
//     NU_ASSERT(handle_data->instance_index < type->instance_count);    
//     nuvk_sdf_instance_data_t *instance = &type->instances[handle_data->instance_index];

//     /* compute new data */
//     nu_quatf_to_mat3(transform->rotation, instance->inv_rotation);
//     nu_mat3f_transpose(instance->inv_rotation);
//     nu_vec3f_copy(transform->translation, instance->translation_scale);
//     instance->translation_scale[3] = transform->scale;

//     /* write buffer */
//     nuvk_sdf_buffer_instances_write_instance_transform(instances_buffer, render_context->active_inflight_frame_index,
//         handle_data->type_index, handle_data->instance_index, instance->inv_rotation, instance->translation_scale);

//     return NU_SUCCESS;
// }
// nu_result_t nuvk_sdf_scene_update_instance_data(
//     nuvk_sdf_scene_t *scene,
//     const nuvk_render_context_t *render_context,
//     nuvk_sdf_buffer_instances_t *instances_buffer,
//     nuvk_sdf_instance_t handle,
//     const void *data
// )
// {
//     uint32_t handle_id;
//     NU_HANDLE_GET_ID(handle, handle_id);
//     nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)nu_indexed_array_get(scene->instance_handles, handle_id);
//     NU_ASSERT(handle_data->type_index < scene->type_count);
//     nuvk_sdf_instance_type_data_t *type = &scene->types[handle_data->type_index];
//     NU_ASSERT(handle_data->instance_index < type->instance_count);    

//     /* copy to local scene data */
//     memcpy((char*)type->data + type->info.data_size * handle_data->instance_index, data, type->info.data_size);

//     /* write buffer */
//     nuvk_sdf_buffer_instances_write_instance_data(instances_buffer, render_context->active_inflight_frame_index,
//         handle_data->type_index, handle_data->instance_index, data);

//     return NU_SUCCESS;
// }