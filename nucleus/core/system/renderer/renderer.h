#ifndef NU_SYSTEM_RENDERER_H
#define NU_SYSTEM_RENDERER_H

#include "../../module/module.h"
#include "../../module/interface.h"
#include "../../event/event.h"

typedef enum {
    NU_RENDERER_API_NONE       = 0,
    NU_RENDERER_API_SOFTRAST   = 1,
    NU_RENDERER_API_RAYTRACER  = 2,
    NU_RENDERER_API_VULKAN     = 3,
    NU_RENDERER_API_OPENGL     = 4
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

/* public renderer interface functions */
NU_API nu_result_t nu_renderer_mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info);
NU_API nu_result_t nu_renderer_mesh_destroy(nu_renderer_mesh_handle_t handle);

NU_API nu_result_t nu_renderer_texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info);
NU_API nu_result_t nu_renderer_texture_destroy(nu_renderer_texture_handle_t handle);

NU_API nu_result_t nu_renderer_material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info);
NU_API nu_result_t nu_renderer_material_destroy(nu_renderer_material_handle_t handle);

NU_API nu_result_t nu_renderer_font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info);
NU_API nu_result_t nu_renderer_font_destroy(nu_renderer_font_handle_t handle);
NU_API nu_result_t nu_renderer_font_get_text_size(nu_renderer_font_handle_t handle, const char *text, nu_vec2u_t size);

NU_API nu_result_t nu_renderer_camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info);
NU_API nu_result_t nu_renderer_camera_destroy(nu_renderer_camera_handle_t handle);
NU_API nu_result_t nu_renderer_camera_set_fov(nu_renderer_camera_handle_t handle, float fov);
NU_API nu_result_t nu_renderer_camera_set_view(nu_renderer_camera_handle_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up);

NU_API nu_result_t nu_renderer_model_create(nu_renderer_model_handle_t *handle, const nu_renderer_model_create_info_t *info);
NU_API nu_result_t nu_renderer_model_destroy(nu_renderer_model_handle_t handle);
NU_API nu_result_t nu_renderer_model_set_transform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform);

NU_API nu_result_t nu_renderer_label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info);
NU_API nu_result_t nu_renderer_label_destroy(nu_renderer_label_handle_t handle);
NU_API nu_result_t nu_renderer_label_set_position(nu_renderer_label_handle_t handle, const nu_vec2i_t position);
NU_API nu_result_t nu_renderer_label_set_text(nu_renderer_label_handle_t handle, const char *text);

NU_API nu_result_t nu_renderer_rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info);
NU_API nu_result_t nu_renderer_rectangle_destroy(nu_renderer_rectangle_handle_t handle);
NU_API nu_result_t nu_renderer_rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect);

NU_API nu_result_t nu_renderer_viewport_set_size(const nu_vec2u_t size);
NU_API nu_result_t nu_renderer_viewport_get_size(nu_vec2u_t size);

/* public renderer event functions */
NU_API nu_event_id_t nu_renderer_viewport_resize_event_get_id(void);

#endif