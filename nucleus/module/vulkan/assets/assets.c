#include <nucleus/module/vulkan/assets/assets.h>

#include <nucleus/module/vulkan/buffer/instances.h>

nu_result_t nuvk_assets_initialize(nuvk_assets_t *assets)
{
    nu_indexed_array_allocate(&assets->sdfs, sizeof(nuvk_sdf_data_t*));
    nu_indexed_array_allocate(&assets->materials, sizeof(nuvk_material_data_t*));
    nu_array_allocate(&assets->material_free_buffer_indices, sizeof(uint32_t));

    return NU_SUCCESS;
}
nu_result_t nuvk_assets_terminate(nuvk_assets_t *assets)
{
    nuvk_sdf_data_t **sdfs;
    uint32_t sdf_count;
    nu_indexed_array_get_data(assets->sdfs, &sdfs, &sdf_count);
    for (uint32_t i = 0; i < sdf_count; i++) {
        nu_free(sdfs[i]);
    }
    nu_indexed_array_free(assets->sdfs);
    
    nuvk_material_data_t **materials;
    uint32_t material_count;
    nu_indexed_array_get_data(assets->materials, &materials, &material_count);
    for (uint32_t i = 0; i < material_count; i++) {
        nu_free(materials[i]);
    }
    nu_indexed_array_free(assets->materials);
    nu_array_free(assets->material_free_buffer_indices);

    return NU_SUCCESS;
}

nu_result_t nuvk_assets_sdf_create(
    nuvk_assets_t *assets,
    nuvk_buffer_instances_t *buffer,
    const nuvk_sdf_info_t *info,
    nuvk_sdf_t *handle
)
{
    nuvk_sdf_data_t *data = (nuvk_sdf_data_t*)nu_malloc(sizeof(nuvk_sdf_data_t));
    data->data_size          = info->data_size;
    data->glsl_data_code     = info->glsl_data_code;
    data->glsl_sdf_code      = info->glsl_sdf_code;
    data->c_sdf_code         = info->c_sdf_code;
    data->c_aabb_code        = info->c_aabb_code;
    data->max_instance_count = info->max_instance_count;
    nu_indexed_array_add(assets->sdfs, &data, &data->id);
    *handle = (nuvk_sdf_t)data;

    /* configure sdfs */
    nuvk_sdf_data_t **sdfs;
    uint32_t sdf_count;
    nu_indexed_array_get_data(assets->sdfs, &sdfs, &sdf_count);
    nuvk_buffer_instances_configure_sdfs(buffer, sdfs, sdf_count); /* buffer_id are set here */

    return NU_SUCCESS;
}
nu_result_t nuvk_assets_material_create(
    nuvk_assets_t *assets,
    nuvk_buffer_materials_t *buffer,
    uint32_t active_inflight_frame_index,
    const nuvk_material_info_t *info,
    nuvk_material_t *handle
)
{
    nuvk_material_data_t *data = (nuvk_material_data_t*)nu_malloc(sizeof(nuvk_material_data_t));
    nu_vec3f_copy(info->color, data->color);
    nu_indexed_array_add(assets->materials, &data, &data->id);
    *handle = (nuvk_material_t)data;

    /* set buffer id */
    if (!nu_array_is_empty(assets->material_free_buffer_indices)) {
        uint32_t *free_id = nu_array_get_last(assets->material_free_buffer_indices);
        data->buffer_id = *free_id;
        nu_array_pop(assets->material_free_buffer_indices);
    } else {
        data->buffer_id = nu_indexed_array_get_size(assets->materials) - 1;
    }

    /* buffer */
    nuvk_buffer_materials_write_material(buffer, active_inflight_frame_index, data->buffer_id, data);

    return NU_SUCCESS;
}