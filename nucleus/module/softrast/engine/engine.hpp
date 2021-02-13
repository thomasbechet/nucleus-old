#pragma once

#include <nucleus/module/softrast/engine/windowinterface.hpp>
#include <nucleus/module/softrast/asset/assetmanager.hpp>
#include <nucleus/module/softrast/memory/colorframebuffer.hpp>
#include <nucleus/module/softrast/scene/scene.hpp>
#include <nucleus/module/softrast/gui/gui.hpp>

#include <unordered_map>

namespace nu::softrast
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
        Vector2u getFontTextSize(nu_renderer_font_handle_t handle, const std::string &text);

        nu_renderer_model_handle_t createModel(const nu_renderer_model_create_info_t &info);
        void destroyModel(nu_renderer_model_handle_t handle);
        void setModelTransform(nu_renderer_model_handle_t handle, const Matrix4f &transform);

        void setCameraFov(float fov);
        void setCameraView(const Vector3f &eye, const Vector3f &forward, const Vector3f &up);

        nu_renderer_label_handle_t createLabel(const nu_renderer_label_create_info_t &info);
        void destroyLabel(nu_renderer_label_handle_t handle);
        void setLabelPosition(nu_renderer_label_handle_t handle, const Vector2i &position);
        void setLabelText(nu_renderer_label_handle_t handle, const std::string &text);

        nu_renderer_rectangle_handle_t createRectangle(const nu_renderer_rectangle_create_info_t &info);
        void destroyRectangle(nu_renderer_rectangle_handle_t handle);
        void setRectangleRect(nu_renderer_rectangle_handle_t handle, const Rect &rect);

        void setViewportSize(const Vector2u &size);
        Vector2u getViewportSize() const;

    private:
        AssetManager m_assetManager;
        WindowInterface m_windowInterface;
        ColorFramebuffer m_colorBuffer;
        Framebuffer<float> m_depthBuffer;
        Scene m_scene;
        GUI m_gui;
    };
}