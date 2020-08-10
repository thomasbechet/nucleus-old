#include "mesh.h"

#define MAX_MESH_COUNT 32

typedef struct {
    nusr_mesh_t **meshes;
    uint32_t next_id;
} nusr_asset_mesh_data_t;

static nusr_asset_mesh_data_t _data;

nu_result_t nusr_mesh_initialize(void)
{
    _data.next_id = 0;
    _data.meshes = (nusr_mesh_t**)nu_malloc(sizeof(nusr_mesh_t*) * MAX_MESH_COUNT);
    memset(_data.meshes, 0, sizeof(nusr_mesh_t*) * MAX_MESH_COUNT);

    return NU_SUCCESS;
}
nu_result_t nusr_mesh_terminate(void)
{
    for (uint32_t i = 0; i < _data.next_id; i++) {
        if (_data.meshes[i]) {
            nusr_mesh_destroy(i);
        }
    }

    nu_free(_data.meshes);

    return NU_SUCCESS;
}

nu_result_t nusr_mesh_create(uint32_t *id, const nusr_mesh_create_info_t *info)
{
    /* error check */
    if (_data.next_id >= MAX_MESH_COUNT) return NU_FAILURE;

    /* create mesh */
    _data.meshes[_data.next_id] = (nusr_mesh_t*)nu_malloc(sizeof(nusr_mesh_t));
    _data.meshes[_data.next_id]->vertex_count = info->vertice_count;

    /* allocate memory */
    _data.meshes[_data.next_id]->positions = (vec3*)nu_malloc(sizeof(vec3) * info->vertice_count);
    _data.meshes[_data.next_id]->uvs = (vec2*)nu_malloc(sizeof(vec2) * info->vertice_count);
    if (info->use_colors) {
        _data.meshes[_data.next_id]->colors = (vec3*)nu_malloc(sizeof(vec3) * info->vertice_count);
    } else {
        _data.meshes[_data.next_id]->colors = NULL;
    }
    
    /* copy data */
    if (info->use_indices) {
        for (uint32_t i = 0; i < info->vertice_count; i++) {
            uint32_t position_indice = info->position_indices[i];
            uint32_t uv_indice = info->uv_indices[i];
            glm_vec3_copy(info->positions[position_indice], _data.meshes[_data.next_id]->positions[i]);
            glm_vec2_copy(info->uvs[uv_indice], _data.meshes[_data.next_id]->uvs[i]);
            
            if (info->use_colors) {
                uint32_t color_indice = info->color_indices[i];
                glm_vec3_copy(info->colors[color_indice], _data.meshes[_data.next_id]->colors[i]);
            }
        }
    } else {
        memcpy(_data.meshes[_data.next_id]->positions, info->positions, sizeof(vec3) * info->vertice_count);
        memcpy(_data.meshes[_data.next_id]->uvs, info->uvs, sizeof(vec2) * info->vertice_count);
        
        if (info->use_colors) {
            memcpy(_data.meshes[_data.next_id]->colors, info->colors, sizeof(vec3) * info->vertice_count);
        } else {
            _data.meshes[_data.next_id]->colors = NULL;
        }
    }

    /* compute min/max positions */
    float xmin, xmax, ymin, ymax, zmin, zmax;
    xmin = xmax = ymin = ymax = zmin = zmax = 0.0f;
    for (uint32_t i = 0; i < _data.meshes[_data.next_id]->vertex_count; i++) {
        float x, y, z;
        x = _data.meshes[_data.next_id]->positions[i][0];
        y = _data.meshes[_data.next_id]->positions[i][1];
        z = _data.meshes[_data.next_id]->positions[i][2];
        xmin = x < xmin ? x : xmin;
        xmax = x > xmax ? x : xmax;
        ymin = y < ymin ? y : ymin;
        ymax = y > ymax ? y : ymax;
        zmin = z < zmin ? z : zmin;
        zmax = z > zmax ? z : zmax;
    }
    _data.meshes[_data.next_id]->xmax = xmax;
    _data.meshes[_data.next_id]->xmin = xmin;
    _data.meshes[_data.next_id]->ymax = ymax;
    _data.meshes[_data.next_id]->ymin = ymin;
    _data.meshes[_data.next_id]->zmax = zmax;
    _data.meshes[_data.next_id]->zmin = zmin;


    /* save id */
    *id = _data.next_id;
    
    _data.next_id++;

    return NU_SUCCESS;
}
nu_result_t nusr_mesh_destroy(uint32_t id)
{
    if (_data.next_id >= MAX_MESH_COUNT) return NU_FAILURE;
    if (!_data.meshes[id]) return NU_FAILURE;

    nu_free(_data.meshes[id]->positions);
    nu_free(_data.meshes[id]->uvs);
    if (_data.meshes[id]->colors) {
        nu_free(_data.meshes[id]->colors);
    }

    nu_free(_data.meshes[id]);
    _data.meshes[id] = NULL;

    return NU_SUCCESS;
}
nu_result_t nusr_mesh_get(uint32_t id, nusr_mesh_t **p)
{
    if (_data.next_id >= MAX_MESH_COUNT) return NU_FAILURE;
    if (!_data.meshes[id]) return NU_FAILURE;

    *p = _data.meshes[id];

    return NU_SUCCESS;
}