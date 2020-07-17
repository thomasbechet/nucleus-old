#ifndef NUSR_MESH_H
#define NUSR_MESH_H

#include "../module/interface.h"
#include "../../core/nucleus_core.h"

typedef struct {
    uint32_t vertex_count;
    vec3 *positions;
    vec2 *uvs;
    vec3 *colors;
    float xmax;
    float xmin;
    float ymax;
    float ymin;
    float zmax;
    float zmin;
} nusr_mesh_t;

nu_result_t nusr_mesh_initialize(void);
nu_result_t nusr_mesh_terminate(void);

nu_result_t nusr_mesh_create(uint32_t *id, const nusr_mesh_create_info_t *info);
nu_result_t nusr_mesh_destroy(uint32_t id);
nu_result_t nusr_mesh_get(uint32_t id, nusr_mesh_t **p);

#endif