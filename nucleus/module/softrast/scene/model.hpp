#pragma once

#include <nucleus/nucleus.hpp>

#include <vector>

namespace nu::softrast
{
    struct Model
    {
        Model(const nu_renderer_model_create_info_t &info);

        nu_renderer_mesh_handle_t mesh;
        std::vector<nu_renderer_material_handle_t> materials;
        Matrix4f transform;
    };
}