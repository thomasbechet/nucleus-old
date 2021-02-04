#pragma once

#include <nucleus/nucleus.h>

namespace nurt
{
    nu_result_t initialize(void);
    nu_result_t terminate(void);
    nu_result_t render(void);

    nu_result_t mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info);
    nu_result_t mesh_destroy(nu_renderer_mesh_handle_t handle);

    nu_result_t texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info);
    nu_result_t texture_destroy(nu_renderer_texture_handle_t handle);

    nu_result_t material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info);
    nu_result_t material_destroy(nu_renderer_material_handle_t handle);

    nu_result_t font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info);
    nu_result_t font_destroy(nu_renderer_font_handle_t handle);
    nu_result_t font_get_text_size(nu_renderer_font_handle_t handle, const char *text, uint32_t *width, uint32_t *height);

    nu_result_t camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info);
    nu_result_t camera_destroy(nu_renderer_camera_handle_t handle);
    nu_result_t camera_set_fov(nu_renderer_camera_handle_t handle, float fov);
    nu_result_t camera_set_view(nu_renderer_camera_handle_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up);
    nu_result_t camera_set_active(nu_renderer_camera_handle_t handle);

    nu_result_t model_create(nu_renderer_model_handle_t *handle, const nu_renderer_model_create_info_t *info);
    nu_result_t model_destroy(nu_renderer_model_handle_t handle);
    nu_result_t model_set_transform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform);

    nu_result_t label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info);
    nu_result_t label_destroy(nu_renderer_label_handle_t handle);
    nu_result_t label_set_position(nu_renderer_label_handle_t handle, const nu_vec2i_t position);
    nu_result_t label_set_text(nu_renderer_label_handle_t handle, const char *text);

    nu_result_t rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info);
    nu_result_t rectangle_destroy(nu_renderer_rectangle_handle_t handle);
    nu_result_t rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect);

    nu_result_t viewport_set_size(uint32_t width, uint32_t height);
    nu_result_t viewport_get_size(uint32_t *width, uint32_t *height);
}