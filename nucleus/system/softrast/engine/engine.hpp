#pragma once

#include "windowinterface.hpp"
#include "../asset/assetmanager.hpp"
#include "../memory/colorframebuffer.hpp"
#include "../scene/scene.hpp"

#include <unordered_map>

namespace nusr
{
    class Engine
    {
    public:
        Engine();

        void render();

        nu_renderer_mesh_handle_t createMesh(const nu_renderer_mesh_create_info_t &info);
        void destroyMesh(nu_renderer_mesh_handle_t handle);

        nu_renderer_texture_handle_t createTexture(const nu_renderer_texture_create_info_t &info);
        void destroyTexture(nu_renderer_texture_handle_t handle);

        nu_renderer_material_handle_t createMaterial(const nu_renderer_material_create_info_t &info);
        void destroyMaterial(nu_renderer_material_handle_t handle);

        nu_renderer_font_handle_t createFont(const nu_renderer_font_create_info_t &info);
        void destroyFont(nu_renderer_font_handle_t handle);
        void getFontTextSize(nu_renderer_font_handle_t handle, const std::string &text, uint32_t &width, uint32_t &height);

        nu_renderer_model_handle_t createModel(const nu_renderer_model_create_info_t &info);
        void destroyModel(nu_renderer_model_handle_t handle);
        void setModelTransform(nu_renderer_model_handle_t handle, const nu_mat4_t transform);

        void setCameraFov(float fov);
        void setCameraEye(const nu_vec3_t eye);
        void setCameraCenter(const nu_vec3_t center);

    private:
        AssetManager m_assetManager;
        WindowInterface m_windowInterface;
        ColorFramebuffer m_colorBuffer;
        Framebuffer<float> m_depthBuffer;
        Scene m_scene;
    };
}