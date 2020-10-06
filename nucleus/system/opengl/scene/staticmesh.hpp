#pragma once

#include "../../../core/nucleus.h"

namespace nugl
{
    class staticmesh_t
    {
    public:
        staticmesh_t(const nu_renderer_staticmesh_create_info_t &info, uint32_t id);
        ~staticmesh_t();

        void get_transform(nu_mat4_t transform) const;
        uint32_t get_mesh() const;
        uint32_t get_material() const;

    private:
        nu_mat4_t m_transform;
        uint32_t m_id;
        uint32_t m_mesh;
        uint32_t m_material;
    };
}