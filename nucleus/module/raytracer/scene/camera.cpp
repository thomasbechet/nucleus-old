#include "camera.hpp"

#include <cmath>

using namespace nurt;

Camera::Camera(
    const nu_vec3f_t eye,
    const nu_vec3f_t center,
    const nu_vec3f_t up,
    float fov,
    float aspect
)
{
    nu_vec3f_copy(eye, m_eye);
    nu_vec3f_copy(center, m_center); 
    nu_vec3f_copy(up, m_up);
    m_fov = fov;
    m_aspect = aspect;

    updateView();
    updateProjection();
}

void Camera::generateRay(float x, float y, nu_vec3f_t pos, nu_vec3f_t dir) const
{
    nu_vec3f_copy(m_eye, pos);

    dir[0] = x * m_fovFactor * m_aspect;
    dir[1] = y * m_fovFactor;
    dir[2] = -1.0f;
    nu_vec3f_normalize(dir);
    nu_mat4f_mulv3(m_viewToWorld, dir, 0.0f, dir); // Apply rotation
}

void Camera::setEye(const nu_vec3f_t eye)
{
    nu_vec3f_copy(eye, m_eye);
    updateView();
}
void Camera::setCenter(const nu_vec3f_t center)
{
    nu_vec3f_copy(center, m_center);
    updateView();
}
void Camera::setUp(const nu_vec3f_t up)
{
    nu_vec3f_copy(up, m_up);
    updateView();
}
void Camera::setFov(float fov)
{
    m_fov = fov;
    updateProjection();
}
void Camera::setAspect(float aspect)
{
    m_aspect = aspect;
    updateProjection();
}

void Camera::updateView()
{
    nu_lookat(m_eye, m_center, m_up, m_viewToWorld);
    nu_mat4f_inv(m_viewToWorld, m_viewToWorld);
}
void Camera::updateProjection()
{
    m_fovFactor = tan(nu_radian(m_fov * 0.5f));
}