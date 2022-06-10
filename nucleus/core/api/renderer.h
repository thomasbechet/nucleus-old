#ifndef NU_API_RENDERER_H
#define NU_API_RENDERER_H

#include <nucleus/core/api/module.h>

NU_DECLARE_HANDLE(nu_renderer_mesh_t);
NU_DECLARE_HANDLE(nu_renderer_font_t);
NU_DECLARE_HANDLE(nu_renderer_texture_t);
NU_DECLARE_HANDLE(nu_renderer_material_t);
NU_DECLARE_HANDLE(nu_renderer_camera_t);
NU_DECLARE_HANDLE(nu_renderer_model_t);
NU_DECLARE_HANDLE(nu_renderer_label_t);
NU_DECLARE_HANDLE(nu_renderer_rectangle_t);

typedef enum {
    NU_RENDERER_MODULE_NONE       = 0,
    NU_RENDERER_MODULE_SOFTRAST   = 1,
    NU_RENDERER_MODULE_VULKAN     = 2
} nu_renderer_module_t;

typedef struct {
    uint32_t vertex_count;
    nu_vec3f_t *positions;
    nu_vec2f_t *uvs;
    nu_vec3f_t *normals;
    nu_vec3f_t *tangents;
    nu_vec3f_t *colors;

    uint32_t indice_count;
    uint32_t *position_indices;
    uint32_t *uv_indices;
    uint32_t *normal_indices;
    uint32_t *tangent_indices;
    uint32_t *color_indices;
} nu_renderer_submesh_info_t;

typedef struct {
    nu_renderer_submesh_info_t *submeshes;
    uint32_t submesh_count;
} nu_renderer_mesh_create_info_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t channel;
    unsigned char *data;
} nu_renderer_texture_create_info_t;

typedef struct {
    nu_renderer_texture_t diffuse_texture;
    nu_renderer_texture_t normal_texture;
    nu_renderer_texture_t specular_texture;
    nu_vec3f_t diffuse_uniform;
    float specular_uniform;
} nu_renderer_material_create_info_t;

typedef struct {
    const char *filename;
    uint32_t font_size;
} nu_renderer_font_create_info_t;

typedef struct {
    nu_mat4f_t view;
    float fov;
} nu_renderer_camera_create_info_t;

typedef struct {
    nu_renderer_mesh_t mesh;
    nu_renderer_material_t *materials;
    uint32_t material_count;
    nu_mat4f_t transform;
} nu_renderer_model_create_info_t;

typedef struct {
    nu_vec2i_t position;
    const char *text;
    nu_renderer_font_t font;
} nu_renderer_label_create_info_t;

typedef struct {
    nu_rect_t rect;
    uint32_t color;
} nu_renderer_rectangle_create_info_t;

typedef struct {
    nu_vec2u_t size;
    nu_vec2u_t old_size;
} nu_renderer_viewport_resize_event_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*start)(void);
    nu_result_t (*stop)(void);
    nu_result_t (*render)(void);
} nu_renderer_system_api_t;

typedef struct {
    nu_renderer_camera_t (*camera_create)(const nu_renderer_camera_create_info_t *info);
    nu_result_t (*camera_destroy)(nu_renderer_camera_t camera);
    void (*camera_set_fov)(nu_renderer_camera_t camera, float fov);
    void (*camera_set_view)(nu_renderer_camera_t camera, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up);
    void (*camera_set_active)(nu_renderer_camera_t camera);

    void (*viewport_set_size)(const nu_vec2u_t size);
    void (*viewport_get_size)(nu_vec2u_t size);
} nu_renderer_api_t;

#endif