#include <nucleus/module/softrast/renderer.hpp>

#include <nucleus/module/softrast/engine/engine.hpp>

#include <memory>

using namespace nu::softrast;

typedef struct {
    std::unique_ptr<Engine> engine;
} nusr_module_data_t;

static nusr_module_data_t _module;

nu_result_t nu::softrast::initialize()
{
    _module.engine = std::make_unique<Engine>();
    return NU_SUCCESS;
}
nu_result_t nu::softrast::terminate()
{
    _module.engine.reset();
    return NU_SUCCESS;
}
nu_result_t nu::softrast::start()
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::stop()
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::render()
{
    _module.engine->render();
    return NU_SUCCESS;
}

nu_result_t nu::softrast::mesh_create(const nu_renderer_mesh_create_info_t *info, nu_renderer_mesh_t *handle)
{
    *handle = _module.engine->createMesh(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::mesh_destroy(nu_renderer_mesh_t handle)
{
    _module.engine->destroyMesh(handle);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::texture_create(const nu_renderer_texture_create_info_t *info, nu_renderer_texture_t *handle)
{
    *handle = _module.engine->createTexture(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::texture_destroy(nu_renderer_texture_t handle)
{
    _module.engine->destroyTexture(handle);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::material_create(const nu_renderer_material_create_info_t *info, nu_renderer_material_t *handle)
{
    *handle = _module.engine->createMaterial(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::material_destroy(nu_renderer_material_t handle)
{
    _module.engine->destroyMaterial(handle);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::font_create(const nu_renderer_font_create_info_t *info, nu_renderer_font_t *handle)
{
    *handle = _module.engine->createFont(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::font_destroy(nu_renderer_font_t handle)
{
    _module.engine->destroyFont(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::font_get_text_size(nu_renderer_font_t handle, const char *text, nu_vec2u_t size)
{
    Vector2u fontSize = _module.engine->getFontTextSize(handle, text);
    nu_vec2u_copy(fontSize, size);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::camera_create(const nu_renderer_camera_create_info_t *info, nu_renderer_camera_t *handle)
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_destroy(nu_renderer_camera_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_set_fov(nu_renderer_camera_t handle, float fov)
{
    _module.engine->setCameraFov(fov);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    _module.engine->setCameraView(eye, forward, up);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::camera_set_active(nu_renderer_camera_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nu::softrast::model_create(const nu_renderer_model_create_info_t *info, nu_renderer_model_t *handle)
{
    *handle = _module.engine->createModel(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::model_destroy(nu_renderer_model_t handle)
{
    _module.engine->destroyModel(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::model_set_transform(nu_renderer_model_t handle, const nu_mat4f_t transform)
{
    _module.engine->setModelTransform(handle, transform);
    return NU_SUCCESS;
}

nu_result_t nu::softrast::label_create(const nu_renderer_label_create_info_t *info, nu_renderer_label_t *handle)
{
    *handle = _module.engine->createLabel(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::label_destroy(nu_renderer_label_t handle)
{
    _module.engine->destroyLabel(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::label_set_position(nu_renderer_label_t handle, const nu_vec2i_t position)
{
    _module.engine->setLabelPosition(handle, position);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::label_set_text(nu_renderer_label_t handle, const char *text)
{
    _module.engine->setLabelText(handle, std::string(text));
    return NU_SUCCESS;
}

nu_result_t nu::softrast::rectangle_create(const nu_renderer_rectangle_create_info_t *info, nu_renderer_rectangle_t *handle)
{
    *handle = _module.engine->createRectangle(*info);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::rectangle_destroy(nu_renderer_rectangle_t handle)
{
    _module.engine->destroyRectangle(handle);
    return NU_SUCCESS;
}
nu_result_t nu::softrast::rectangle_set_rect(nu_renderer_rectangle_t handle, const nu_rect_t rect)
{
    _module.engine->setRectangleRect(handle, Rect(rect));
    return NU_SUCCESS;
}

nu_result_t nu::softrast::viewport_set_size(const nu_vec2u_t size)
{
    _module.engine->setViewportSize(Vector2u(size));
    return NU_SUCCESS;
}
nu_result_t nu::softrast::viewport_get_size(nu_vec2u_t size)
{
    nu_vec2u_copy(_module.engine->getViewportSize(), size);
    return NU_SUCCESS;
}