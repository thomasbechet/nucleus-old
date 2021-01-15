#include "engine.hpp"

#include "../common/config.h"

#include <limits>

using namespace nusr;

Engine::Engine()
{
    uint32_t default_width, default_height;
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_WIDTH, 640, &default_width);
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_HEIGHT, 360, &default_height);

    m_colorBuffer = ColorFramebuffer(default_width, default_height);
    m_depthBuffer = Framebuffer<float>(default_width, default_height);
}

void Engine::render()
{
    // Clear buffers
    m_colorBuffer.clear(0x0);
    m_depthBuffer.clear(std::numeric_limits<float>::infinity());

    // Draw scene
    m_scene.draw(m_colorBuffer, m_depthBuffer, m_assetManager);

    // Present
    m_colorBuffer.display(m_windowInterface);
}

nu_renderer_mesh_handle_t Engine::createMesh(const nu_renderer_mesh_create_info_t &info)
{
    return m_assetManager.createMesh(info);
}
void Engine::destroyMesh(nu_renderer_mesh_handle_t handle)
{
    m_assetManager.destroyMesh(handle);
}

nu_renderer_texture_handle_t Engine::createTexture(const nu_renderer_texture_create_info_t &info)
{
    return m_assetManager.createTexture(info);
}
void Engine::destroyTexture(nu_renderer_texture_handle_t handle)
{
    m_assetManager.destroyTexture(handle);
}

nu_renderer_material_handle_t Engine::createMaterial(const nu_renderer_material_create_info_t &info)
{
    return m_assetManager.createMaterial(info);
}
void Engine::destroyMaterial(nu_renderer_material_handle_t handle)
{
    m_assetManager.destroyMaterial(handle);
}

nu_renderer_font_handle_t Engine::createFont(const nu_renderer_font_create_info_t &info)
{
    return m_assetManager.createFont(info);
}
void Engine::destroyFont(nu_renderer_font_handle_t handle)
{
    m_assetManager.destroyFont(handle);
}
void Engine::getFontTextSize(nu_renderer_font_handle_t handle, const std::string &text, uint32_t &width, uint32_t &height)
{
    const Font &font = m_assetManager.getFont(handle);
    font.getTextSize(text, width, height);
}

nu_renderer_model_handle_t Engine::createModel(const nu_renderer_model_create_info_t &info)
{
    return m_scene.createModel(info);
}
void Engine::destroyModel(nu_renderer_model_handle_t handle)
{
    m_scene.destroyModel(handle);
}
void Engine::setModelTransform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform)
{
    m_scene.setModelTransform(handle, transform);
}

void Engine::setCameraFov(float fov)
{
    m_scene.setCameraFov(fov);
}
void Engine::setCameraView(const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    m_scene.setCameraView(eye, forward, up);
}