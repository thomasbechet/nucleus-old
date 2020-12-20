#include "object.hpp"

using namespace nurt;

Object::Object(
    const nu_mat4_t transform,
    MaterialHandle material
)
{
    nu_mat4_copy(transform, m_transform);
    m_material = material;
}

void Object::setTransform(const nu_mat4_t transform)
{
    nu_mat4_copy(transform, m_transform);
    onTransformUpdated();
}