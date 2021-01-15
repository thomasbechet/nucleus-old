#include "material.hpp"

using namespace nugl;

material_t::material_t(const nu_renderer_material_create_info_t &info)
{
    m_use_diffuse_uniform = info.use_diffuse_uniform;
    if (!m_use_diffuse_uniform) {
        m_diffuse_texture = NU_HANDLE_GET_ID(info.diffuse_texture);
    }
}

void material_t::bind(const material_t &material, const std::unordered_map<uint32_t, std::unique_ptr<texture_t>> &textures)
{
    if (!material.m_use_diffuse_uniform) {
        auto &diffuse_texture = *textures.at(material.m_diffuse_texture).get();
        diffuse_texture.bind(0);
    }
}