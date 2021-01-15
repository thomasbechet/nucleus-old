#include "renderer.hpp"

#include "engine/engine.hpp"

#include <memory>

using namespace nusr;

typedef struct {
    std::unique_ptr<Engine> engine;
} nusr_data_t;

static nusr_data_t _data;

nu_result_t nusr::initialize()
{
    _data.engine = std::make_unique<Engine>();
    return NU_SUCCESS;
}
nu_result_t nusr::terminate()
{
    _data.engine.reset();
    return NU_SUCCESS;
}
nu_result_t nusr::render()
{
    _data.engine->render();
    return NU_SUCCESS;
}

nu_result_t nusr::mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info)
{
    *handle = _data.engine->createMesh(*info);
    return NU_SUCCESS;
}
nu_result_t nusr::mesh_destroy(nu_renderer_mesh_handle_t handle)
{
    _data.engine->destroyMesh(handle);
    return NU_SUCCESS;
}

nu_result_t nusr::texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info)
{
    *handle = _data.engine->createTexture(*info);
    return NU_SUCCESS;
}
nu_result_t nusr::texture_destroy(nu_renderer_texture_handle_t handle)
{
    _data.engine->destroyTexture(handle);
    return NU_SUCCESS;
}

nu_result_t nusr::material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info)
{
    *handle = _data.engine->createMaterial(*info);
    return NU_SUCCESS;
}
nu_result_t nusr::material_destroy(nu_renderer_material_handle_t handle)
{
    _data.engine->destroyMaterial(handle);
    return NU_SUCCESS;
}

nu_result_t nusr::font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    *handle = _data.engine->createFont(*info);
    return NU_SUCCESS;
}
nu_result_t nusr::font_destroy(nu_renderer_font_handle_t handle)
{
    _data.engine->destroyFont(handle);
    return NU_SUCCESS;
}
nu_result_t nusr::font_get_text_size(nu_renderer_font_handle_t handle, const char *text, uint32_t *width, uint32_t *height)
{
    _data.engine->getFontTextSize(handle, text, *width, *height);
    return NU_SUCCESS;
}

nu_result_t nusr::camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nusr::camera_destroy(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nusr::camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    _data.engine->setCameraFov(fov);
    return NU_SUCCESS;
}
nu_result_t nusr::camera_set_view(nu_renderer_camera_handle_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    _data.engine->setCameraView(eye, forward, up);
    return NU_SUCCESS;
}
nu_result_t nusr::camera_set_active(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nusr::model_create(nu_renderer_model_handle_t *handle, const nu_renderer_model_create_info_t *info)
{
    *handle = _data.engine->createModel(*info);
    return NU_SUCCESS;
}
nu_result_t nusr::model_destroy(nu_renderer_model_handle_t handle)
{
    _data.engine->destroyModel(handle);
    return NU_SUCCESS;
}
nu_result_t nusr::model_set_transform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform)
{
    _data.engine->setModelTransform(handle, transform);
    return NU_SUCCESS;
}

nu_result_t nusr::label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nusr::label_destroy(nu_renderer_label_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nusr::label_set_position(nu_renderer_label_handle_t handle, int32_t x, int32_t y)
{
    return NU_SUCCESS;
}
nu_result_t nusr::label_set_text(nu_renderer_label_handle_t handle, const char *text)
{
    return NU_SUCCESS;
}

nu_result_t nusr::rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nusr::rectangle_destroy(nu_renderer_rectangle_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nusr::rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect)
{
    return NU_SUCCESS;
}

nu_result_t nusr::viewport_set_size(uint32_t width, uint32_t height)
{
    return NU_SUCCESS;
}
nu_result_t nusr::viewport_get_size(uint32_t *width, uint32_t *height)
{
    return NU_SUCCESS;
}