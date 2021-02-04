#pragma once

#include <nucleus/nucleus.hpp>

#include <vector>

namespace nu::softrast
{
    struct Vertex
    {
        Vector3f position;
        Vector2f uv;
    };

    struct Mesh
    {
        Mesh(const nu_renderer_mesh_create_info_t &info);

        std::vector<std::vector<Vertex>> submeshes;
        float xmax;
        float xmin;
        float ymax;
        float ymin;
        float zmax;
        float zmin;
    };
}