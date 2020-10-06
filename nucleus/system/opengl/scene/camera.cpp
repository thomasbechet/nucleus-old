#include "camera.hpp"

using namespace nugl;

camera_t::camera_t()
{
    nu_vec3_zero(m_eye);
    nu_vec3_zero(m_center);
    nu_vec3_t forward = {0, 0, -1};
    nu_vec3_copy(m_center, forward);
    nu_vec3_t up = {0.0f, 1.0f, 0.0f};
    nu_vec3_copy(m_up, up);
    m_fov = 90.0f;
    m_near = 0.1f;
    m_far = 1000.0f;
    m_aspect = (16.0f / 9.0f);
    m_dirty = true;
}

void camera_t::set_fov(float fov)
{
    m_fov = fov;
    m_dirty = true;
}
void camera_t::set_eye(const nu_vec3_t eye)
{
    nu_vec3_copy(eye, m_eye);
    m_dirty = true;
}
void camera_t::set_center(const nu_vec3_t center)
{
    nu_vec3_copy(center, m_center);
    m_dirty = true;
}
void camera_t::set_aspect(float aspect)
{
    m_aspect = aspect;
    m_dirty = true;
}

void camera_t::get_view_matrix(nu_mat4_t matrix)
{
    if (m_dirty) compute_vp_matrix();
    nu_mat4_copy(m_view_matrix, matrix);
}
void camera_t::get_projection_matrix(nu_mat4_t matrix)
{
    if (m_dirty) compute_vp_matrix();
    nu_mat4_copy(m_projection_matrix, matrix);
}

void camera_t::compute_vp_matrix()
{
    nu_vec3_t up = {0.0f, 1.0f, 0.0f};
    nu_lookat(m_eye, m_center, up, m_view_matrix);
    nu_perspective(m_fov, m_aspect, m_near, m_far, m_projection_matrix);
    m_dirty = false;
}