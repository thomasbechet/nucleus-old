#pragma once

#include "../memory/colorframebuffer.hpp"
#include "../asset/assetmanager.hpp"
#include "camera.hpp"
#include "model.hpp"

#include <unordered_map>

namespace nusr
{
    class Scene
    {
    public:
        Scene();

        void setCameraFov(float fov);
        void setCameraEye(const nu_vec3_t eye);
        void setCameraCenter(const nu_vec3_t center);

        nu_renderer_model_handle_t createModel(const nu_renderer_model_create_info_t &info);
        void destroyModel(nu_renderer_model_handle_t handle);
        void setModelTransform(nu_renderer_model_handle_t handle, const nu_mat4_t transform);

        void draw(ColorFramebuffer &colorBuffer, Framebuffer<float> &depthBuffer, const AssetManager &assetManager) const;

    private:
        Camera m_camera;
        std::unordered_map<uint32_t, Model> m_models;
        uint32_t m_nextId = 0;
    };
}