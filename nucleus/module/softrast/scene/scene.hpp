#pragma once

#include "../memory/colorframebuffer.hpp"
#include "../asset/assetmanager.hpp"
#include "camera.hpp"
#include "model.hpp"

#include <unordered_map>

namespace nu::softrast
{
    class Scene
    {
    public:
        Scene();

        void setCameraFov(float fov);
        void setCameraView(const Vector3f &eye, const Vector3f &forward, const Vector3f &up);

        nu_renderer_model_handle_t createModel(const nu_renderer_model_create_info_t &info);
        void destroyModel(nu_renderer_model_handle_t handle);
        void setModelTransform(nu_renderer_model_handle_t handle, const nu::Matrix4f &transform);

        void draw(ColorFramebuffer &colorBuffer, Framebuffer<float> &depthBuffer, const AssetManager &assetManager) const;

    private:
        Camera m_camera;
        std::unordered_map<uint32_t, Model> m_models;
        uint32_t m_nextId = 0;
    };
}