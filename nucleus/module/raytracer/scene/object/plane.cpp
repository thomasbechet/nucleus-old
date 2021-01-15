#include "plane.hpp"

#include <cmath>

using namespace nurt;

Plane::Plane(
    const nu_mat4f_t transform,
    MaterialHandle material
) : Object(transform, material)
{
    onTransformUpdated();
}

bool Plane::intersect(const nu_vec3f_t pos, const nu_vec3f_t dir, HitInfo &hitInfo) const
{
    float denom, t;
    nu_vec3f_t temp;

    denom = nu_vec3f_dot(dir, m_normal);
    
    if (fabsf(denom) > 0.0001f)
    {
        nu_vec3f_sub(m_position, pos, temp);
        t = nu_vec3f_dot(temp, m_normal) / denom;
        if (t >= 0.0f)
        {
            // Position
            nu_vec3f_copy(dir, hitInfo.position);
            nu_vec3f_muls(hitInfo.position, t, hitInfo.position);
            nu_vec3f_add(hitInfo.position, pos, hitInfo.position);

            // Normal
            nu_vec3f_copy(m_normal, hitInfo.normal);

            // Distance
            hitInfo.distance = t;

            // Material
            hitInfo.material = m_material;

            return true;
        }
    }

    return false;
}
void Plane::onTransformUpdated()
{
    nu_vec3f_t position = {0.0f, 0.0f, 0.0f};
    nu_mat4f_mulv3(m_transform, position, 1.0f, m_position); // translation and rotation
    nu_vec3f_t normal = {0.0f, 1.0f, 0.0f};
    nu_mat4f_mulv3(m_transform, normal, 0.0f, m_normal); // rotation only
}