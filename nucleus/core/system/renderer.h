#ifndef NU_SYSTEM_RENDERER_H
#define NU_SYSTEM_RENDERER_H

#include "../module/module.h"
#include "../module/interface.h"

typedef enum {
    NU_RENDERER_API_NONE       = 0,
    NU_RENDERER_API_SOFTRAST   = 1,
    NU_RENDERER_API_VULKAN     = 2,
    NU_RENDERER_API_OPENGL     = 3
} nu_renderer_api_t;

typedef struct {
    bool vertical_synchronization;
} nu_renderer_create_info_t;

/* private system functions */
nu_result_t nu_system_renderer_initialize(void);
nu_result_t nu_system_renderer_terminate(void);
nu_result_t nu_system_renderer_render(void);

/* public system functions */
NU_API nu_module_handle_t nu_system_renderer_get_module_handle(void);

/* public renderer functions */
NU_API nu_result_t nu_renderer_mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info);
NU_API nu_result_t nu_renderer_mesh_destroy(nu_renderer_mesh_handle_t handle);

NU_API nu_result_t nu_renderer_texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info);
NU_API nu_result_t nu_renderer_texture_destroy(nu_renderer_texture_handle_t handle);

NU_API nu_result_t nu_renderer_font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info);
NU_API nu_result_t nu_renderer_font_destroy(nu_renderer_font_handle_t handle);

NU_API nu_result_t nu_renderer_camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info);
NU_API nu_result_t nu_renderer_camera_destroy(nu_renderer_camera_handle_t handle);
NU_API nu_result_t nu_renderer_camera_set_fov(nu_renderer_camera_handle_t handle, float fov);
NU_API nu_result_t nu_renderer_camera_set_eye(nu_renderer_camera_handle_t handle, const nu_vec3_t eye);
NU_API nu_result_t nu_renderer_camera_set_center(nu_renderer_camera_handle_t handle, const nu_vec3_t center);

NU_API nu_result_t nu_renderer_staticmesh_create(nu_renderer_staticmesh_handle_t *handle, const nu_renderer_staticmesh_create_info_t *info);
NU_API nu_result_t nu_renderer_staticmesh_destroy(nu_renderer_staticmesh_handle_t handle);
NU_API nu_result_t nu_renderer_staticmesh_set_transform(nu_renderer_staticmesh_handle_t handle, const nu_mat4_t transform);

NU_API nu_result_t nu_renderer_label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info);
NU_API nu_result_t nu_renderer_label_destroy(nu_renderer_label_handle_t handle);
NU_API nu_result_t nu_renderer_label_set_position(nu_renderer_label_handle_t handle, int32_t x, int32_t y);
NU_API nu_result_t nu_renderer_label_set_text(nu_renderer_label_handle_t handle, const char *text);

#endif