#include "object.hpp"

using namespace nurt;

Object::Object(
    const nu_mat4f_t transform,
    MaterialHandle material
)
{
    nu_mat4f_copy(transform, m_transform);
    m_material = material;
}

void Object::setTransform(const nu_mat4f_t transform)
{
    nu_mat4f_copy(transform, m_transform);
    onTransformUpdated();
}