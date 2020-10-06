#ifndef NU_SYSTEM_RENDERER_INTERFACE_H
#define NU_SYSTEM_RENDERER_INTERFACE_H

#include "../../common/common.h"

/* renderer system interface */
#define NU_RENDERER_INTERFACE_NAME        "nu_renderer_interface"
#define NU_RENDERER_INTERFACE_LOADER_NAME "nu_renderer_get_interface"

NU_DECLARE_HANDLE(nu_renderer_mesh_handle_t);
NU_DECLARE_HANDLE(nu_renderer_font_handle_t);
NU_DECLARE_HANDLE(nu_renderer_texture_handle_t);
NU_DECLARE_HANDLE(nu_renderer_material_handle_t);
NU_DECLARE_HANDLE(nu_renderer_camera_handle_t);
NU_DECLARE_HANDLE(nu_renderer_staticmesh_handle_t);
NU_DECLARE_HANDLE(nu_renderer_label_handle_t);
NU_DECLARE_HANDLE(nu_renderer_rectangle_handle_t);

typedef struct {
    uint32_t vertex_count;
    nu_vec3_t *positions;
    nu_vec2_t *uvs;
    nu_vec3_t *normals;
    nu_vec3_t *tangents;
    nu_vec3_t *colors;

    uint32_t indice_count;
    uint32_t *position_indices;
    uint32_t *uv_indices;
    uint32_t *normal_indices;
    uint32_t *tangent_indices;
    uint32_t *color_indices;
} nu_renderer_mesh_create_info_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t channel;
    unsigned char *data;
} nu_renderer_texture_create_info_t;

typedef struct {
    bool use_diffuse_uniform;
    bool use_normal_uniform;
    bool use_specular_uniform;
    nu_renderer_texture_handle_t diffuse_texture;
    nu_renderer_texture_handle_t normal_texture;
    nu_renderer_texture_handle_t specular_texture;
    nu_vec3_t diffuse_uniform;
    float specular_uniform;
} nu_renderer_material_create_info_t;

typedef struct {
    const char *filename;
    uint32_t font_size;
} nu_renderer_font_create_info_t;

typedef struct {
    nu_vec3_t eye;
    nu_vec3_t center;
    nu_vec3_t up;
    float fov;
} nu_renderer_camera_create_info_t;

typedef struct {
    nu_renderer_mesh_handle_t mesh;
    nu_renderer_material_handle_t material;
    nu_mat4_t transform;
} nu_renderer_staticmesh_create_info_t;

typedef struct {
    uint32_t x;
    uint32_t y;
    const char *text;
    nu_renderer_font_handle_t font;
} nu_renderer_label_create_info_t;

typedef struct {
    nu_rect_t rect;
    uint32_t color;
} nu_renderer_rectangle_create_info_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*render)(void);

    nu_result_t (*mesh_create)(nu_renderer_mesh_handle_t*, const nu_renderer_mesh_create_info_t*);
    nu_result_t (*mesh_destroy)(nu_renderer_mesh_handle_t);

    nu_result_t (*texture_create)(nu_renderer_texture_handle_t*, const nu_renderer_texture_create_info_t*);
    nu_result_t (*texture_destroy)(nu_renderer_texture_handle_t);

    nu_result_t (*material_create)(nu_renderer_material_handle_t*, const nu_renderer_material_create_info_t*);
    nu_result_t (*material_destroy)(nu_renderer_material_handle_t);

    nu_result_t (*font_create)(nu_renderer_font_handle_t*, const nu_renderer_font_create_info_t*);
    nu_result_t (*font_destroy)(nu_renderer_font_handle_t);
    nu_result_t (*font_get_text_size)(nu_renderer_font_handle_t, const char*, uint32_t*, uint32_t*);

    nu_result_t (*camera_create)(nu_renderer_camera_handle_t*, const nu_renderer_camera_create_info_t*);
    nu_result_t (*camera_destroy)(nu_renderer_camera_handle_t);
    nu_result_t (*camera_set_fov)(nu_renderer_camera_handle_t, float);
    nu_result_t (*camera_set_eye)(nu_renderer_camera_handle_t, const nu_vec3_t);
    nu_result_t (*camera_set_center)(nu_renderer_camera_handle_t, const nu_vec3_t);
    nu_result_t (*camera_set_active)(nu_renderer_camera_handle_t);

    nu_result_t (*staticmesh_create)(nu_renderer_staticmesh_handle_t*, const nu_renderer_staticmesh_create_info_t*);
    nu_result_t (*staticmesh_destroy)(nu_renderer_staticmesh_handle_t);
    nu_result_t (*staticmesh_set_transform)(nu_renderer_staticmesh_handle_t, const nu_mat4_t);

    nu_result_t (*label_create)(nu_renderer_label_handle_t*, const nu_renderer_label_create_info_t*);
    nu_result_t (*label_destroy)(nu_renderer_label_handle_t);
    nu_result_t (*label_set_position)(nu_renderer_label_handle_t, int32_t, int32_t);
    nu_result_t (*label_set_text)(nu_renderer_label_handle_t, const char*);

    nu_result_t (*rectangle_create)(nu_renderer_rectangle_handle_t*, const nu_renderer_rectangle_create_info_t*);
    nu_result_t (*rectangle_destroy)(nu_renderer_rectangle_handle_t);
    nu_result_t (*rectangle_set_rect)(nu_renderer_rectangle_handle_t, nu_rect_t);

    nu_result_t (*viewport_set_size)(uint32_t, uint32_t);
    nu_result_t (*viewport_get_size)(uint32_t*, uint32_t*);
} nu_renderer_interface_t;

typedef nu_result_t (*nu_renderer_interface_loader_pfn_t)(nu_renderer_interface_t*);

/* system renderer event */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t old_width;
    uint32_t old_height;
} nu_renderer_viewport_resize_event_t;

#endif