#include "staticmesh.hpp"

using namespace nugl;

staticmesh_t::staticmesh_t(
    uint32_t id, 
    uint32_t mesh, 
    uint32_t material, 
    const nu_mat4_t transform
)
{
    m_id = id;
    m_mesh = mesh;
    m_material = material;
    nu_mat4_copy(transform, m_transform);
}
staticmesh_t::~staticmesh_t()
{

}

void staticmesh_t::get_transform(nu_mat4_t transform) const
{
    nu_mat4_copy(m_transform, transform);
}
uint32_t staticmesh_t::get_mesh() const
{
    return m_mesh;
}
uint32_t staticmesh_t::get_material() const
{
    return m_material;
}