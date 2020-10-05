#pragma once

#include "../../../core/nucleus.h"

namespace nugl
{
    class camera_t
    {
    public:
        camera_t();

        void set_fov(float fov);
        void set_eye(const nu_vec3_t eye);
        void set_center(const nu_vec3_t center);
        void set_aspect(float aspect);

        void get_vp_matrix(nu_mat4_t matrix);

        void get_view_matrix(nu_mat4_t matrix);
        void get_projection_matrix(nu_mat4_t matrix);

    private:
        void compute_vp_matrix();

        nu_vec3_t m_eye;
        nu_vec3_t m_center;
        nu_vec3_t m_up;
        float m_fov;
        float m_near;
        float m_far;
        float m_aspect;
        nu_mat4_t m_view_matrix;
        nu_mat4_t m_projection_matrix;
        bool m_dirty;
    };
}