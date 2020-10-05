#pragma once

#include "../../../core/nucleus.h"

#include <GL/glew.h>

#include <array>
#include <vector>

namespace nugl
{
    class mesh_t
    {
    public:
        mesh_t(const nu_renderer_mesh_create_info_t &info);
        ~mesh_t();

        void draw_buffer() const;

    private:
        void compute_normal_and_tangent();

        GLuint m_vao;
        GLuint m_position_vbo;
        GLuint m_uv_vbo;
        GLuint m_normal_vbo;
        GLuint m_tangent_vbo;
        GLuint m_color_vbo;
        uint32_t m_count;

        std::vector<std::array<float, 3>> m_positions;
        std::vector<std::array<float, 2>> m_uvs;
        std::vector<std::array<float, 3>> m_normals;
        std::vector<std::array<float, 3>> m_tangents;
        std::vector<std::array<float, 3>> m_colors;
    };
}