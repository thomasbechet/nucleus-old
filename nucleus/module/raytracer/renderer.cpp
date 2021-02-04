#include "renderer.hpp"

#include "engine/raytracer.hpp"

#include <memory>

using namespace nurt;

typedef struct {
    std::unique_ptr<Raytracer> raytracer;
} nurt_data_t;

static nurt_data_t _data;

nu_result_t nurt::initialize(void)
{
    _data.raytracer = std::make_unique<Raytracer>();
    return NU_SUCCESS;
}
nu_result_t nurt::terminate(void)
{
    _data.raytracer.reset();
    return NU_SUCCESS;
}
nu_result_t nurt::render(void)
{
    _data.raytracer->render();
    return NU_SUCCESS;
}

nu_result_t nurt::mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::mesh_destroy(nu_renderer_mesh_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nurt::texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::texture_destroy(nu_renderer_texture_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nurt::material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::material_destroy(nu_renderer_material_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nurt::font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::font_destroy(nu_renderer_font_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nurt::font_get_text_size(nu_renderer_font_handle_t handle, const char *text, uint32_t *width, uint32_t *height)
{
    return NU_SUCCESS;
}

nu_result_t nurt::camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::camera_destroy(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nurt::camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    return NU_SUCCESS;
}
nu_result_t nurt::camera_set_view(nu_renderer_camera_handle_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    return NU_SUCCESS;
}
nu_result_t nurt::camera_set_active(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nurt::model_create(nu_renderer_model_handle_t *handle, const nu_renderer_model_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::model_destroy(nu_renderer_model_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nurt::model_set_transform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform)
{
    return NU_SUCCESS;
}

nu_result_t nurt::label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::label_destroy(nu_renderer_label_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nurt::label_set_position(nu_renderer_label_handle_t handle, const nu_vec2i_t position)
{
    return NU_SUCCESS;
}
nu_result_t nurt::label_set_text(nu_renderer_label_handle_t handle, const char *text)
{
    return NU_SUCCESS;
}

nu_result_t nurt::rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nurt::rectangle_destroy(nu_renderer_rectangle_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nurt::rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect)
{
    return NU_SUCCESS;
}

nu_result_t nurt::viewport_set_size(uint32_t width, uint32_t height)
{
    return NU_SUCCESS;
}
nu_result_t nurt::viewport_get_size(uint32_t *width, uint32_t *height)
{
    return NU_SUCCESS;
}