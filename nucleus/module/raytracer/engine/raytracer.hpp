#pragma once

#include "windowinterface.hpp"
#include "colorframebuffer.hpp"
#include "prng.hpp"
#include "../asset/assetmanager.hpp"
#include "../scene/scene.ipp"
#include "../scene/camera.hpp"

#include <memory>

namespace nurt
{
    class Raytracer
    {
    public:
        Raytracer();

        void render();

    private:
        void castRay(const nu_vec3f_t pos, const nu_vec3f_t dir, uint32_t depth, nu_vec3f_t color) const;
        void createCoordinateSystem(const nu_vec3f_t N, nu_vec3f_t Nt, nu_vec3f_t Nb) const;
        void uniformSampleHemisphere(float r1, float r2, nu_vec3f_t dir) const;

    private:
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_sampleCount;
        uint32_t m_maxDepth;
        float m_bias;
        bool m_saveImage;

        bool m_done;

        std::unique_ptr<WindowInterface> m_windowInterface;
        std::unique_ptr<ColorFramebuffer> m_colorBuffer;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<Scene> m_scene;
        std::unique_ptr<Camera> m_camera;

        mutable PRNG m_prng;
    };
}