#include "mesh.hpp"

#include "../common/logger.h"

using namespace nusr;

Mesh::Mesh(const nu_renderer_mesh_create_info_t &info)
{
    xmin = xmax = ymin = ymax = zmin = zmax = 0.0f;

    submeshes.resize(info.submesh_count);

    for (uint32_t meshIndex = 0; meshIndex < info.submesh_count; meshIndex++) {
        nu_renderer_submesh_info_t &meshInfo = info.submeshes[meshIndex];
        std::vector<Vertex> &vertices = submeshes[meshIndex];
        
        uint32_t vertex_count = (meshInfo.indice_count > 0) ? meshInfo.indice_count : meshInfo.vertex_count;
        vertices.reserve(vertex_count);

        // Copy data
        if (meshInfo.indice_count > 0) {
            for (uint32_t i = 0; i < meshInfo.indice_count; i++) {
                Vertex vertex;
                uint32_t position_indice = meshInfo.position_indices[i];
                uint32_t uv_indice = meshInfo.uv_indices[i];
                nu_vec3f_copy(meshInfo.positions[position_indice], vertex.position);
                nu_vec2f_copy(meshInfo.uvs[uv_indice], vertex.uv);
                vertices.emplace_back(vertex);
            }
        } else {
            for (uint32_t i = 0; i < meshInfo.vertex_count; i++) {
                Vertex vertex;
                nu_vec3f_copy(meshInfo.positions[i], vertex.position);
                nu_vec2f_copy(meshInfo.uvs[i], vertex.uv);
                vertices.emplace_back(vertex);
            }
        }

        // Compute min/max coordinates
        for (Vertex v : vertices) {
            float x, y, z;
            x = v.position[0];
            y = v.position[1];
            z = v.position[2];
            xmin = x < xmin ? x : xmin;
            xmax = x > xmax ? x : xmax;
            ymin = y < ymin ? y : ymin;
            ymax = y > ymax ? y : ymax;
            zmin = z < zmin ? z : zmin;
            zmax = z > zmax ? z : zmax;
        }
    }
}