#include "sphere.hpp"

#include <cmath>

using namespace nurt;

Sphere::Sphere(
    const nu_mat4_t transform,
    MaterialHandle material,
    float radius
) : Object(transform, material), m_radius(radius)
{
    onTransformUpdated();
}

bool Sphere::intersect(const nu_vec3_t pos, const nu_vec3_t dir, HitInfo &hitInfo) const
{
    nu_vec3_t L;
    float tca, d2, thc, t0;

    nu_vec3_sub(m_center, pos, L);
    tca = nu_vec3_dot(L, dir);
    if (tca < 0.0f) return false;
    d2 = nu_vec3_dot(L, L) - tca * tca;
    if (d2 > m_radius) return false;
    thc = sqrt(m_radius - d2);
    t0 = tca - thc;

    // Position
    nu_vec3_copy(dir, hitInfo.position);
    nu_vec3_muls(hitInfo.position, t0, hitInfo.position);
    nu_vec3_add(hitInfo.position, pos, hitInfo.position);

    // Normal
    nu_vec3_copy(hitInfo.position, hitInfo.normal);
    nu_vec3_sub(hitInfo.normal, m_center, hitInfo.normal);
    nu_vec3_normalize(hitInfo.normal);

    // Distance
    hitInfo.distance = t0;

    // Material
    hitInfo.material = m_material;

    return true;
}
void Sphere::onTransformUpdated()
{
    nu_vec3_t center = {0.0f, 0.0f, 0.0f};
    nu_mat4_mulv3(m_transform, center, 1.0f, m_center);
}