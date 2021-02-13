#include <nucleus/module/softrast/asset/mesh.hpp>

using namespace nu::softrast;

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
                uint32_t uv_indice       = meshInfo.uv_indices[i];
                vertex.position = Vector3f(meshInfo.positions[position_indice]);
                vertex.uv       = Vector2f(meshInfo.uvs[uv_indice]);
                vertices.emplace_back(vertex);
            }
        } else {
            for (uint32_t i = 0; i < meshInfo.vertex_count; i++) {
                Vertex vertex;
                vertex.position = Vector3f(meshInfo.positions[i]);
                vertex.uv       = Vector2f(meshInfo.uvs[i]);
                vertices.emplace_back(vertex);
            }
        }

        // Compute min/max coordinates
        for (Vertex v : vertices) {
            float x, y, z;
            x = v.position.x;
            y = v.position.y;
            z = v.position.z;
            xmin = x < xmin ? x : xmin;
            xmax = x > xmax ? x : xmax;
            ymin = y < ymin ? y : ymin;
            ymax = y > ymax ? y : ymax;
            zmin = z < zmin ? z : zmin;
            zmax = z > zmax ? z : zmax;
        }
    }
}