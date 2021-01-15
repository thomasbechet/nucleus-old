#pragma once

#include "../../../core/nucleus.h"

namespace nugl
{
    class staticmesh_t
    {
    public:
        staticmesh_t(
            uint32_t id, 
            uint32_t mesh, 
            uint32_t material,
            const nu_mat4f_t transform
        );
        ~staticmesh_t();

        void get_transform(nu_mat4f_t transform) const;
        uint32_t get_mesh() const;
        uint32_t get_material() const;

    private:
        uint32_t m_id;
        uint32_t m_mesh;
        uint32_t m_material;
        nu_mat4f_t m_transform;
    };
}