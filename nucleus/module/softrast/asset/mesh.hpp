#pragma once

#include "../module/interface.h"

#include <vector>

namespace nusr
{
    struct Vertex
    {
        nu_vec3f_t position;
        nu_vec2f_t uv;
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