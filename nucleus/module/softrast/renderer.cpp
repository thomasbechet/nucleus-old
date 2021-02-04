#include "renderer.hpp"

#include "engine/engine.hpp"

#include <memory>

using namespace nu::softrast;

typedef struct {
    std::unique_ptr<Engine> engine;
} nusr_data_t;

static nusr_data_t _data;

nu_result_t nu::softrast::initialize()
{
    _data.engine = std::make_unique<Engine>();
    return NU_SUCCESS;
}
nu_result_t nu::softrast::terminate()
{
    _data.engine.reset();
    return NU_SUCCESS;
}
nu_result_t nu::softrast::render()
{
    _data.engine->render();
    return NU_SUCCESS;
}

nu_result_t nu::softrast::mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info)
{
    *handle = _data.engine->createMesh(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::mesh_destroy(nu_renderer_mesh_handle_t handle)
{
    _data.engine->destroyMesh(handle);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info)
{
    *handle = _data.engine->createTexture(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::texture_destroy(nu_renderer_texture_handle_t handle)
{
    _data.engine->destroyTexture(handle);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info)
{
    *handle = _data.engine->createMaterial(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::material_destroy(nu_renderer_material_handle_t handle)
{
    _data.engine->destroyMaterial(handle);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    *handle = _data.engine->createFont(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::font_destroy(nu_renderer_font_handle_t handle)
{
    _data.engine->destroyFont(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::font_get_text_size(nu_renderer_font_handle_t handle, const char *text, nu_vec2u_t size)
{
    Vector2u fontSize = _data.engine->getFontTextSize(handle, text);
    nu_vec2u_copy(fontSize, size);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_destroy(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    _data.engine->setCameraFov(fov);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_set_view(nu_renderer_camera_handle_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    _data.engine->setCameraView(eye, forward, up);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_set_active(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nu::softrast::model_create(nu_renderer_model_handle_t *handle, const nu_renderer_model_create_info_t *info)
{
    *handle = _data.engine->createModel(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::model_destroy(nu_renderer_model_handle_t handle)
{
    _data.engine->destroyModel(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::model_set_transform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform)
{
    _data.engine->setModelTransform(handle, transform);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info)
{
    *handle = _data.engine->createLabel(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::label_destroy(nu_renderer_label_handle_t handle)
{
    _data.engine->destroyLabel(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::label_set_position(nu_renderer_label_handle_t handle, const nu_vec2i_t position)
{
    _data.engine->setLabelPosition(handle, position);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::label_set_text(nu_renderer_label_handle_t handle, const char *text)
{
    _data.engine->setLabelText(handle, std::string(text));
    return NU_SUCCESS;
}

nu_result_t nu::softrast::rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::rectangle_destroy(nu_renderer_rectangle_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::rectangle_set_rect(nu_renderer_rectangle_handle_t handle, const nu_rect_t rect)
{
    return NU_SUCCESS;
}

nu_result_t nu::softrast::viewport_set_size(const nu_vec2u_t size)
{
    _data.engine->setViewportSize(Vector2u(size));
    return NU_SUCCESS;
}
nu_result_t nu::softrast::viewport_get_size(nu_vec2u_t size)
{
    nu_vec2u_copy(_data.engine->getViewportSize(), size);
    return NU_SUCCESS;
}