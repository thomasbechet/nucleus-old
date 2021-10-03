#include <nucleus/module/vulkan/sdf/scene/scene.h>

nu_result_t nuvk_sdf_scene_initialize(nuvk_sdf_scene_t *scene)
{
    nuvk_sdf_camera_initialize(&scene->camera);
    nu_indexed_array_allocate(&scene->instance_handles, sizeof(nuvk_sdf_instance_handle_data_t));
    scene->type_count     = 0;
    scene->material_count = 0;
    memset(scene->material_update_table, 0, sizeof(uint32_t) * NUVK_SDF_MAX_MATERIAL_COUNT);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_terminate(nuvk_sdf_scene_t *scene)
{
    for (uint32_t t = 0; t < scene->type_count; t++) {
        nu_free(scene->types[t].instances);
        nu_free(scene->types[t].data);
        nu_array_free(scene->types[t].free_instance_indices);
        nu_free(scene->types[t].indices);
        nu_free(scene->types[t].instance_update_table);
    }

    nu_indexed_array_free(scene->instance_handles);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_update_buffers(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_buffer_environment_t *environment_buffer,
    nuvk_sdf_buffer_instances_t *instances_buffer
)
{
    /* update camera buffer */
    nuvk_sdf_buffer_environment_write_camera(environment_buffer, &scene->camera,
        render_context->active_inflight_frame_index);

    /* update indice and instance buffers */
    for (uint32_t t = 0; t < scene->type_count; t++) {
        nuvk_sdf_instance_type_data_t *type = &scene->types[t];
        
        /* update index count */
        nuvk_sdf_buffer_instances_write_index_count(instances_buffer, render_context->active_inflight_frame_index,
            t, type->index_count);

        /* update index, transform and instance data if needed */
        for (uint32_t i = 0; i < type->index_count; i++) {
            if (type->instance_update_table[i]) {
                uint32_t instance_index            = type->indices[i];
                nuvk_sdf_instance_data_t *instance = &type->instances[instance_index];
                
                nuvk_sdf_buffer_instances_write_index(instances_buffer, render_context->active_inflight_frame_index,
                    t, i, instance_index);

                nuvk_sdf_buffer_instances_write_instance_transform(instances_buffer, render_context->active_inflight_frame_index,
                    t, instance_index, instance->inv_rotation, instance->translation_scale);

                nuvk_sdf_buffer_instances_write_instance_data(instances_buffer, render_context->active_inflight_frame_index,
                    t, instance_index, (char*)type->data + instance_index * type->info.data_size);

                type->instance_update_table[i]--;
            }
        }
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_scene_create_material(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_material_info_t *info,
    nuvk_sdf_material_t *handle
)
{


    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_destroy_material(
    nuvk_sdf_scene_t *scene,
    nuvk_sdf_material_t handle
)
{


    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_scene_register_instance_type(
    nuvk_sdf_scene_t *scene,
    const nuvk_sdf_instance_type_info_t *info,
    nuvk_sdf_instance_type_t *handle
)
{
    NU_CHECK(scene->type_count < NUVK_SDF_MAX_INSTANCE_TYPE_COUNT, return NU_FAILURE,
        NUVK_LOGGER_NAME, "Max sdf type count reached.");

    NU_HANDLE_SET_ID(*handle, scene->type_count);
    nuvk_sdf_instance_type_data_t *type = &scene->types[scene->type_count++];

    /* setup and allocate memory */
    type->info = *info;
    
    type->instances      = (nuvk_sdf_instance_data_t*)nu_malloc(sizeof(nuvk_sdf_instance_data_t) * info->max_instance_count);
    type->data           = nu_malloc(info->data_size * info->max_instance_count);
    type->instance_count = 0;
    nu_array_allocate_capacity(&type->free_instance_indices, sizeof(uint32_t), info->max_instance_count);

    type->indices     = (uint32_t*)nu_malloc(sizeof(uint32_t) * info->max_instance_count);
    type->index_count = 0;

    type->instance_update_table = (uint32_t*)nu_malloc(sizeof(uint32_t) * info->max_instance_count);
    memset(type->instance_update_table, 0, sizeof(uint32_t) * info->max_instance_count);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_create_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_instance_info_t *info,
    nuvk_sdf_instance_t *handle
)
{
    uint32_t type_index;
    NU_HANDLE_GET_ID(info->type, type_index);
    NU_ASSERT(type_index < scene->type_count);
    nuvk_sdf_instance_type_data_t *type = &scene->types[type_index];
    
    NU_ASSERT(type->instance_count <= type->info.max_instance_count);

    /* recover instance index */
    uint32_t instance_index;
    if (!nu_array_is_empty(type->free_instance_indices)) {
        instance_index = *(uint32_t*)nu_array_get_last(type->free_instance_indices);
        nu_array_pop(type->free_instance_indices);
    } else {
        instance_index = type->instance_count++;
    }

    /* setup instance */
    nuvk_sdf_instance_data_t *instance = &type->instances[instance_index];
    instance->transform = info->transform;
    
    nu_quatf_to_mat3(info->transform.rotation, instance->inv_rotation);
    nu_mat3f_transpose(instance->inv_rotation);
    nu_vec3f_copy(info->transform.translation, instance->translation_scale);
    instance->translation_scale[3] = info->transform.scale;

    /* update indices and notify buffer */
    instance->index_position = type->index_count;
    type->indices[instance->index_position] = instance_index;
    type->index_count++;
    type->instance_update_table[instance->index_position] = render_context->max_inflight_frame_count;

    /* save data */
    void *pdata = (char*)type->data + instance->index_position * type->info.data_size;
    memcpy(pdata, info->data, type->info.data_size);

    /* setup handle */
    nuvk_sdf_instance_handle_data_t handle_data;
    handle_data.type_index     = type_index;
    handle_data.instance_index = instance_index;
    uint32_t id;
    nu_indexed_array_add(scene->instance_handles, &handle_data, &id);
    NU_HANDLE_SET_ID(*handle, id);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_destroy_instance(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_instance_t handle
)
{
    uint32_t handle_id;
    NU_HANDLE_GET_ID(handle, handle_id);
    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)nu_indexed_array_get(scene->instance_handles, handle_id);
    (void)handle_data;

    nu_warning(NUVK_LOGGER_NAME, "Destroying instance has not been implemented yet.");

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_update_instance_transform(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_instance_t handle,
    const nuvk_sdf_transform_t *transform
)
{
    uint32_t handle_id;
    NU_HANDLE_GET_ID(handle, handle_id);
    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)nu_indexed_array_get(scene->instance_handles, handle_id);
    NU_ASSERT(handle_data->type_index < scene->type_count);
    nuvk_sdf_instance_type_data_t *type = &scene->types[handle_data->type_index];
    NU_ASSERT(handle_data->instance_index < type->instance_count);    
    nuvk_sdf_instance_data_t *instance = &type->instances[handle_data->instance_index];

    nu_quatf_to_mat3(transform->rotation, instance->inv_rotation);
    nu_mat3f_transpose(instance->inv_rotation);
    nu_vec3f_copy(transform->translation, instance->translation_scale);
    instance->translation_scale[3] = transform->scale;

    type->instance_update_table[instance->index_position] = render_context->max_inflight_frame_count;

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_scene_update_instance_data(
    nuvk_sdf_scene_t *scene,
    const nuvk_render_context_t *render_context,
    nuvk_sdf_instance_t handle,
    const void *data
)
{
    uint32_t handle_id;
    NU_HANDLE_GET_ID(handle, handle_id);
    nuvk_sdf_instance_handle_data_t *handle_data = (nuvk_sdf_instance_handle_data_t*)nu_indexed_array_get(scene->instance_handles, handle_id);
    NU_ASSERT(handle_data->type_index < scene->type_count);
    nuvk_sdf_instance_type_data_t *type = &scene->types[handle_data->type_index];
    NU_ASSERT(handle_data->instance_index < type->instance_count);    
    nuvk_sdf_instance_data_t *instance = &type->instances[handle_data->instance_index];

    memcpy((char*)type->data + type->info.data_size * handle_data->instance_index, data, type->info.data_size);

    type->instance_update_table[instance->index_position] = render_context->max_inflight_frame_count;

    return NU_SUCCESS;
}