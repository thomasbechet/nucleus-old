#ifndef NUSR_INTERFACE_H
#define NUSR_INTERFACE_H

#include "../../core/nucleus_core.h"

#define NUSR_MODULE_NAME "nucleus-softrast"
#define NUSR_MODULE_ID 0x7

#define NUSR_RENDERER_INTERFACE_NAME        "nusr_renderer_interface"
#define NUSR_RENDERER_INTERFACE_LOADER_NAME "nusr_renderer_interface_loader"

typedef struct {
    uint32_t vertice_count;
    vec3 *positions;
    vec2 *uvs;
    vec3 *colors;
    uint32_t *indices;
} nusr_mesh_create_info_t;

typedef struct {
    vec3 eye;
    vec3 center;
    vec3 up;
    float fov;
} nusr_camera_create_info_t;

typedef struct {
    uint32_t mesh;
    mat4 transform;
} nusr_staticmesh_create_info_t;

typedef struct {
    nu_result_t (*mesh_create)(uint32_t*, const nusr_mesh_create_info_t*);
    nu_result_t (*mesh_destroy)(uint32_t);

    nu_result_t (*camera_set_fov)(float);
    nu_result_t (*camera_set_eye)(const vec3);
    nu_result_t (*camera_set_center)(const vec3);

    nu_result_t (*staticmesh_create)(uint32_t*, const nusr_staticmesh_create_info_t*);
    nu_result_t (*staticmesh_destroy)(uint32_t);
    nu_result_t (*staticmesh_set_transform)(uint32_t, const mat4);
} nusr_renderer_interface_t;

typedef nu_result_t (*nusr_renderer_interface_loader_pfn_t)(nusr_renderer_interface_t*);

#endif