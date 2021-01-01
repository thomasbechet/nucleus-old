#include "loader.hpp"

#include "logger.hpp"

#include <unordered_map>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

struct MeshVertices
{
    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 2>> uvs;
    std::vector<std::array<float, 3>> colors;
};

nu_result_t load_mesh_from_obj(nu_renderer_mesh_handle_t *handle, const char *filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, filename, true);
    if(!ret)
    {
        err = (NUUTILS_LOGGER_NAME"tinyobj Error: " + err);
        nu_warning(err.c_str());
        return NU_FAILURE;
    }

    std::unordered_map<int, MeshVertices> meshes;
    for(size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];
            int material_id =  shapes[s].mesh.material_ids[f];

            for(size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                std::array<float, 3> position;
                position[0] = attrib.vertices[3 * idx.vertex_index + 0];
                position[1] = attrib.vertices[3 * idx.vertex_index + 1];
                position[2] = attrib.vertices[3 * idx.vertex_index + 2];
                
                std::array<float, 2> uv;
                if(!attrib.texcoords.empty())
                {
                    uv[0] = attrib.texcoords[2 * idx.texcoord_index + 0];
                    uv[1] = attrib.texcoords[2 * idx.texcoord_index + 1];
                }

                // if(!attrib.normals.empty())
                // {
                //     vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
                //     vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
                //     vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];                    
                // }

                std::array<float, 3> color;
                color[0] = attrib.colors[3 * idx.vertex_index + 0];
                color[1] = attrib.colors[3 * idx.vertex_index + 1];
                color[2] = attrib.colors[3 * idx.vertex_index + 2];

                meshes[material_id].positions.push_back(position);
                meshes[material_id].uvs.push_back(uv);
                meshes[material_id].colors.push_back(color);
            }

            index_offset += fv;
        }
    }

    auto first = meshes.begin(); //TODO : load multi mesh

    nu_renderer_submesh_info_t submeshInfo = {0};
    submeshInfo.vertex_count     = first->second.positions.size();
    submeshInfo.positions        = (nu_vec3_t*)first->second.positions.data();
    submeshInfo.uvs              = (nu_vec2_t*)first->second.uvs.data();
    submeshInfo.colors           = (nu_vec3_t*)first->second.colors.data();
    submeshInfo.indice_count     = 0;
    nu_renderer_mesh_create_info_t info = {0};
    info.submeshes     = &submeshInfo;
    info.submesh_count = 1;

    if (nu_renderer_mesh_create(handle, &info) != NU_SUCCESS) {
        nu_warning(NUUTILS_LOGGER_NAME"Failed to load mesh.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t load_texture(nu_renderer_texture_handle_t *handle, const char *filename)
{
    return NU_SUCCESS;
}

nu_result_t nuutils_loader_get_interface(nuutils_loader_interface_t *interface)
{
    interface->load_mesh_from_obj = load_mesh_from_obj;
    interface->load_texture       = load_texture;

    return NU_SUCCESS;
}