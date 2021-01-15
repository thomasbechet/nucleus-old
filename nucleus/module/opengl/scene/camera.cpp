#include "camera.hpp"

using namespace nugl;

camera_t::camera_t()
{
    nu_mat4f_identity(m_view_matrix);
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
void camera_t::set_view(const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    nu_vec3f_copy(eye, m_eye);
    nu_vec3f_t center;
    nu_vec3f_add(eye, forward, center);
    nu_vec3f_copy(center, m_center);
    nu_vec3f_copy(up, m_up);
    m_dirty = true;
}
void camera_t::set_aspect(float aspect)
{
    m_aspect = aspect;
    m_dirty = true;
}

void camera_t::get_view_matrix(nu_mat4f_t matrix)
{
    if (m_dirty) compute_vp_matrix();
    nu_mat4f_copy(m_view_matrix, matrix);
}
void camera_t::get_projection_matrix(nu_mat4f_t matrix)
{
    if (m_dirty) compute_vp_matrix();
    nu_mat4f_copy(m_projection_matrix, matrix);
}

void camera_t::compute_vp_matrix()
{
    nu_lookat(m_eye, m_center, m_up, m_view_matrix);
    nu_perspective(m_fov, m_aspect, m_near, m_far, m_projection_matrix);
    m_dirty = false;
}