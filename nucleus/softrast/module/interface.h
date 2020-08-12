#ifndef NUSR_INTERFACE_H
#define NUSR_INTERFACE_H

#include "../../core/nucleus_core.h"

#define NUSR_MODULE_NAME "nucleus-softrast"
#define NUSR_MODULE_ID 0x7

#define NUSR_RENDERER_INTERFACE_NAME        "nusr_renderer_interface"
#define NUSR_RENDERER_INTERFACE_LOADER_NAME "nusr_renderer_interface_loader"

typedef struct {
    uint32_t vertice_count;
    bool use_indices;
    bool use_colors;
    nu_vec3_t *positions;
    nu_vec2_t *uvs;
    nu_vec3_t *colors;
    uint32_t *position_indices;
    uint32_t *uv_indices;
    uint32_t *color_indices;
} nusr_mesh_create_info_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t channel;
    unsigned char *data;
} nusr_texture_create_info_t;

typedef struct {
    nu_vec3_t eye;
    nu_vec3_t center;
    nu_vec3_t up;
    float fov;
} nusr_camera_create_info_t;

typedef struct {
    uint32_t mesh;
    uint32_t texture;
    nu_mat4_t transform;
} nusr_staticmesh_create_info_t;

typedef struct {
    nu_result_t (*mesh_create)(uint32_t*, const nusr_mesh_create_info_t*);
    nu_result_t (*mesh_destroy)(uint32_t);

    nu_result_t (*camera_set_fov)(float);
    nu_result_t (*camera_set_eye)(const nu_vec3_t);
    nu_result_t (*camera_set_center)(const nu_vec3_t);

    nu_result_t (*staticmesh_create)(uint32_t*, const nusr_staticmesh_create_info_t*);
    nu_result_t (*staticmesh_destroy)(uint32_t);
    nu_result_t (*staticmesh_set_transform)(uint32_t, const nu_mat4_t);
} nusr_renderer_interface_t;

typedef nu_result_t (*nusr_renderer_interface_loader_pfn_t)(nusr_renderer_interface_t*);

#endif