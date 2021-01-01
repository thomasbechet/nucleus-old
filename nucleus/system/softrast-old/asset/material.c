#include "material.h"

#include "../common/logger.h"

#define MAX_MATERIAL_COUNT 50

typedef struct {
    nusr_material_t **materials;
    uint32_t next_id;
} nusr_asset_material_data_t;

static nusr_asset_material_data_t _data;

static nu_result_t create_material(uint32_t *id, const nu_renderer_material_create_info_t *info)
{
    /* error check */
    if (_data.next_id >= MAX_MATERIAL_COUNT) return NU_FAILURE;

    /* create mesh */
    _data.materials[_data.next_id] = (nusr_material_t*)nu_malloc(sizeof(nusr_material_t));
    _data.materials[_data.next_id]->use_diffuse_uniform = info->use_diffuse_uniform;
    if (!info->use_diffuse_uniform) {
        _data.materials[_data.next_id]->diffuse_texture = NU_HANDLE_GET_ID(info->diffuse_texture);
    }

    /* save id */
    *id = _data.next_id++;

    return NU_SUCCESS;
}
static nu_result_t destroy_material(uint32_t id)
{
    if (_data.next_id >= MAX_MATERIAL_COUNT) return NU_FAILURE;
    if (!_data.materials[id]) return NU_FAILURE;

    nu_free(_data.materials[id]);
    _data.materials[id] = NULL;

    return NU_SUCCESS;
}

nu_result_t nusr_material_initialize(void)
{
    _data.next_id = 0;
    _data.materials = (nusr_material_t**)nu_malloc(sizeof(nusr_material_t*) * MAX_MATERIAL_COUNT);
    memset(_data.materials, 0, sizeof(nusr_material_t*) * MAX_MATERIAL_COUNT);

    return NU_SUCCESS;
}
nu_result_t nusr_material_terminate(void)
{
    for (uint32_t i = 0; i < _data.next_id; i++) {
        if (_data.materials[i]) {
            destroy_material(i);
        }
    }

    nu_free(_data.materials);

    return NU_SUCCESS;
}

nu_result_t nusr_material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info)
{
    uint32_t id;
    nu_result_t result = create_material(&id, info);
    if (result == NU_SUCCESS) NU_HANDLE_SET_ID(*handle, id);
    return result;

    return NU_SUCCESS;
}
nu_result_t nusr_material_destroy(nu_renderer_material_handle_t handle)
{
    uint32_t id = NU_HANDLE_GET_ID(handle);
    return destroy_material(id);

    return NU_SUCCESS;
}
nu_result_t nusr_material_get(uint32_t id, nusr_material_t **p)
{
    if (id >= MAX_MATERIAL_COUNT) return NU_FAILURE;
    if (!_data.materials[id]) return NU_FAILURE;

    *p = _data.materials[id];

    return NU_SUCCESS;
}