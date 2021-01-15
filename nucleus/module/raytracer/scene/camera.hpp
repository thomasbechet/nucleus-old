#pragma once

#include <nucleus/nucleus.h>

namespace nurt
{
    class Camera
    {
    public:
        Camera(
            const nu_vec3f_t eye,
            const nu_vec3f_t center,
            const nu_vec3f_t up,
            float fov = 70.0f,
            float aspect = 16.0f / 9.0f
        );

        void generateRay(float x, float y, nu_vec3f_t pos, nu_vec3f_t dir) const;

        void setEye(const nu_vec3f_t eye);
        void setCenter(const nu_vec3f_t center);
        void setUp(const nu_vec3f_t up);
        void setFov(float fov);
        void setAspect(float aspect);

    private:
        void updateView();
        void updateProjection();

        nu_vec3f_t m_eye;
        nu_vec3f_t m_center;
        nu_vec3f_t m_up;
        float m_fov;
        float m_aspect;
        
        float m_fovFactor;
        nu_mat4f_t m_viewToWorld;
    };
}