#pragma once

#include <nucleus/nucleus.h>

#include <vector>

namespace nusr
{
    class Model
    {
    public:
        Model(const nu_renderer_model_create_info_t &info);

        nu_renderer_mesh_handle_t mesh;
        std::vector<nu_renderer_material_handle_t> materials;
        nu_mat4f_t transform;
    };
}