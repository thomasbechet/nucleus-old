#include "mesh.hpp"

using namespace nugl;

mesh_t::mesh_t(const nu_renderer_mesh_create_info_t &info)
{
    if (info.indice_count) {
        m_count = info.indice_count;
    } else {
        m_count = info.vertex_count;
    }

    m_positions.resize(m_count);
    m_uvs.resize(m_count);
    m_normals.resize(m_count);
    m_tangents.resize(m_count);
    m_colors.resize(m_count);

    if (info.indice_count) {
        if (info.position_indices) {
            for (uint32_t i = 0; i < m_count; i++) {
                nu_vec3_copy(info.positions[info.position_indices[i]], (float*)&m_positions[i]);
            }
        }
        if (info.uv_indices) {
            for (uint32_t i = 0; i < m_count; i++) {
                nu_vec2_copy(info.uvs[info.uv_indices[i]], (float*)&m_uvs[i]);
            }
        }
        if (info.normal_indices) {
            for (uint32_t i = 0; i < m_count; i++) {
                nu_vec3_copy(info.normals[info.normal_indices[i]], (float*)&m_normals[i]);
            }
        }
        if (info.tangent_indices) {
            for (uint32_t i = 0; i < m_count; i++) {
                nu_vec3_copy(info.tangents[info.uv_indices[i]], (float*)&m_tangents[i]);
            }
        }
        if (info.color_indices) {
            for (uint32_t i = 0; i < m_count; i++) {
                nu_vec3_copy(info.colors[info.color_indices[i]], (float*)&m_colors[i]);
            }
        }
    } else {
        if (info.positions) memcpy(&m_positions[0], &info.positions[0], m_count * sizeof(nu_vec3_t));
        if (info.uvs) memcpy(&m_uvs[0], &info.uvs[0], m_count * sizeof(nu_vec2_t));
        if (info.normals) memcpy(&m_normals[0], &info.normals[0], m_count * sizeof(nu_vec3_t));
        if (info.tangents) memcpy(&m_tangents[0], &info.tangents[0], m_count * sizeof(nu_vec3_t));
        if (info.colors) memcpy(&m_colors[0], &info.colors[0], m_count * sizeof(nu_vec3_t));
    }

    compute_normal_and_tangent();

    // if (!info.normals) compute_normal();
    // if (!info.tangents) compute_tangent();

    /* vbo */
    glGenBuffers(1, &m_position_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_position_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nu_vec3_t) * m_positions.size(), m_positions.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nu_vec2_t) * m_uvs.size(), m_uvs.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nu_vec3_t) * m_normals.size(), m_normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_tangent_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tangent_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nu_vec3_t) * m_tangents.size(), m_tangents.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nu_vec3_t) * m_colors.size(), m_colors.data(), GL_STATIC_DRAW);
    
    /* vao */
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_position_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_tangent_vbo);
    glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, m_color_vbo);
    glVertexAttribPointer(4, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
mesh_t::~mesh_t()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_position_vbo);
    glDeleteBuffers(1, &m_uv_vbo);
    glDeleteBuffers(1, &m_normal_vbo);
    glDeleteBuffers(1, &m_tangent_vbo);
    glDeleteBuffers(1, &m_color_vbo);
}

void mesh_t::draw_buffer() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_count);
    glBindVertexArray(0);
}

void mesh_t::compute_normal_and_tangent()
{
    for (uint32_t i = 0; i < m_positions.size(); i += 3) {
        /* normal */
        float *v0 = (float*)&m_positions[i + 0];
        float *v1 = (float*)&m_positions[i + 1];
        float *v2 = (float*)&m_positions[i + 2];
    
        nu_vec3_t dv1, dv2;
        nu_vec3_sub(v1, v0, dv1);
        nu_vec3_sub(v2, v0, dv2);

        nu_vec3_t normal;
        nu_vec3_cross(dv1, dv2, normal);
        nu_vec3_normalize(normal);

        nu_vec3_copy(normal, (float*)&m_normals[i + 0]);
        nu_vec3_copy(normal, (float*)&m_normals[i + 1]);
        nu_vec3_copy(normal, (float*)&m_normals[i + 2]);

        /* tangent */
        float *uv0 = (float*)&m_uvs[i + 0];
        float *uv1 = (float*)&m_uvs[i + 1];
        float *uv2 = (float*)&m_uvs[i + 2];

        nu_vec2_t duv1, duv2;
        nu_vec2_sub(uv1, uv0, duv1);
        nu_vec2_sub(uv2, uv0, duv2);

        nu_vec3_t tangent;
        float r = 1.0f / (duv1[0] * duv2[1] - duv1[1] * duv2[0]);
        nu_vec3_muls(dv1, duv2[1], dv1);
        nu_vec3_muls(dv2, duv1[1], dv2);
        nu_vec3_sub(dv1, dv2, tangent);
        nu_vec3_muls(tangent, r, tangent);
        
        nu_vec3_copy(tangent, (float*)&m_tangents[i + 0]);
        nu_vec3_copy(tangent, (float*)&m_tangents[i + 1]);
        nu_vec3_copy(tangent, (float*)&m_tangents[i + 2]);
    }
}
        