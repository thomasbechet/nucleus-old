#include "staticmesh.hpp"

using namespace nugl;

staticmesh_t::staticmesh_t(const nu_renderer_staticmesh_create_info_t &info, uint32_t id)
{
    m_id = id;
    m_mesh = NU_HANDLE_GET_ID(info.mesh);
    m_material = NU_HANDLE_GET_ID(info.material);
    nu_mat4_copy(info.transform, m_transform);
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