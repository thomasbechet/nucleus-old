#pragma once

#include <nucleus/module/softrast/asset/mesh.hpp>
#include <nucleus/module/softrast/asset/texture.hpp>
#include <nucleus/module/softrast/asset/material.hpp>
#include <nucleus/module/softrast/asset/font.hpp>

#include <unordered_map>
#include <memory>

namespace nu::softrast
{
    class AssetManager
    {
    public:
        nu_renderer_mesh_handle_t createMesh(const nu_renderer_mesh_create_info_t &info);
        void destroyMesh(nu_renderer_mesh_handle_t handle);
        const Mesh &getMesh(nu_renderer_mesh_handle_t handle) const;

        nu_renderer_texture_handle_t createTexture(const nu_renderer_texture_create_info_t &info);
        void destroyTexture(nu_renderer_texture_handle_t handle);
        const Texture &getTexture(nu_renderer_texture_handle_t handle) const;

        nu_renderer_material_handle_t createMaterial(const nu_renderer_material_create_info_t &info);
        void destroyMaterial(nu_renderer_material_handle_t handle);
        const Material &getMaterial(nu_renderer_material_handle_t handle) const;

        nu_renderer_font_handle_t createFont(const nu_renderer_font_create_info_t &info);
        void destroyFont(nu_renderer_font_handle_t handle);
        const Font &getFont(nu_renderer_font_handle_t handle) const;

    private:
        std::unordered_map<uint32_t, std::unique_ptr<Mesh>> m_meshes;
        std::unordered_map<uint32_t, std::unique_ptr<Texture>> m_textures;
        std::unordered_map<uint32_t, std::unique_ptr<Material>> m_materials;
        std::unordered_map<uint32_t, std::unique_ptr<Font>> m_fonts;
        uint32_t m_nextId = 0;
    };
}