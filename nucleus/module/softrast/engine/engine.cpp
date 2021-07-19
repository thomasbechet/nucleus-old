#include "engine.hpp"

#include <nucleus/module/softrast/common/config.hpp>

#include <limits>

using namespace nu;
using namespace nu::softrast;

Engine::Engine()
{
    Vector2u defaultSize;
    nu_config_get_uint(Config::Section.c_str(), Config::FramebufferWidth.c_str(), 640, &defaultSize.x);
    nu_config_get_uint(Config::Section.c_str(), Config::FramebufferHeight.c_str(), 360, &defaultSize.y);

    m_colorBuffer = ColorFramebuffer(defaultSize);
    m_depthBuffer = Framebuffer<float>(defaultSize);
}

void Engine::render()
{
    // Clear buffers
    m_colorBuffer.clear(0x0);
    m_depthBuffer.clear(std::numeric_limits<float>::infinity());

    // Draw scene
    m_scene.draw(m_colorBuffer, m_depthBuffer, m_assetManager);
    // Draw GUI
    m_gui.draw(m_colorBuffer, m_assetManager);

    // Present
    m_colorBuffer.display(m_windowInterface);
}

nu_renderer_mesh_t Engine::createMesh(const nu_renderer_mesh_create_info_t &info)
{
    return m_assetManager.createMesh(info);
}
void Engine::destroyMesh(nu_renderer_mesh_t handle)
{
    m_assetManager.destroyMesh(handle);
}

nu_renderer_texture_t Engine::createTexture(const nu_renderer_texture_create_info_t &info)
{
    return m_assetManager.createTexture(info);
}
void Engine::destroyTexture(nu_renderer_texture_t handle)
{
    m_assetManager.destroyTexture(handle);
}

nu_renderer_material_t Engine::createMaterial(const nu_renderer_material_create_info_t &info)
{
    return m_assetManager.createMaterial(info);
}
void Engine::destroyMaterial(nu_renderer_material_t handle)
{
    m_assetManager.destroyMaterial(handle);
}

nu_renderer_font_t Engine::createFont(const nu_renderer_font_create_info_t &info)
{
    return m_assetManager.createFont(info);
}
void Engine::destroyFont(nu_renderer_font_t handle)
{
    m_assetManager.destroyFont(handle);
}
Vector2u Engine::getFontTextSize(nu_renderer_font_t handle, const std::string &text)
{
    const Font &font = m_assetManager.getFont(handle);
    return font.getTextSize(text);
}

nu_renderer_model_t Engine::createModel(const nu_renderer_model_create_info_t &info)
{
    return m_scene.createModel(info);
}
void Engine::destroyModel(nu_renderer_model_t handle)
{
    m_scene.destroyModel(handle);
}
void Engine::setModelTransform(nu_renderer_model_t handle, const Matrix4f &transform)
{
    m_scene.setModelTransform(handle, transform);
}

void Engine::setCameraFov(float fov)
{
    m_scene.setCameraFov(fov);
}
void Engine::setCameraView(const Vector3f &eye, const Vector3f &forward, const Vector3f &up)
{
    m_scene.setCameraView(eye, forward, up);
}

nu_renderer_label_t Engine::createLabel(const nu_renderer_label_create_info_t &info)
{
    return m_gui.createLabel(info);
}
void Engine::destroyLabel(nu_renderer_label_t handle)
{
    m_gui.destroyLabel(handle);
}
void Engine::setLabelPosition(nu_renderer_label_t handle, const Vector2i &position)
{
    m_gui.setLabelPosition(handle, position);
}
void Engine::setLabelText(nu_renderer_label_t handle, const std::string &text)
{
    m_gui.setLabelText(handle, text);
}

nu_renderer_rectangle_t Engine::createRectangle(const nu_renderer_rectangle_create_info_t &info)
{
    return m_gui.createRectangle(info);
}
void Engine::destroyRectangle(nu_renderer_rectangle_t handle)
{
    m_gui.destroyRectangle(handle);
}
void Engine::setRectangleRect(nu_renderer_rectangle_t handle, const Rect &rect)
{
    m_gui.setRectangleRect(handle, rect);
}

void Engine::setViewportSize(const Vector2u &size)
{
    m_colorBuffer = ColorFramebuffer(size);
    m_depthBuffer = Framebuffer<float>(size);
}
Vector2u Engine::getViewportSize() const
{
    return m_colorBuffer.getSize();
}