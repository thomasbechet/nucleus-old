#include "assetmanager.hpp"

using namespace nu::softrast;

nu_renderer_mesh_handle_t AssetManager::createMesh(const nu_renderer_mesh_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_meshes.emplace(id, std::make_unique<Mesh>(info));
    nu_renderer_mesh_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyMesh(nu_renderer_mesh_handle_t handle)
{
    m_meshes.erase(NU_HANDLE_GET_ID(handle));
}
const Mesh &AssetManager::getMesh(nu_renderer_mesh_handle_t handle) const
{
    return *m_meshes.at(NU_HANDLE_GET_ID(handle)).get();
}

nu_renderer_texture_handle_t AssetManager::createTexture(const nu_renderer_texture_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_textures.emplace(id, std::make_unique<Texture>(info));
    nu_renderer_texture_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyTexture(nu_renderer_texture_handle_t handle)
{
    m_textures.erase(NU_HANDLE_GET_ID(handle));
}
const Texture &AssetManager::getTexture(nu_renderer_texture_handle_t handle) const
{
    return *m_textures.at(NU_HANDLE_GET_ID(handle)).get();
}

nu_renderer_material_handle_t AssetManager::createMaterial(const nu_renderer_material_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_materials.emplace(id, std::make_unique<Material>(info));
    nu_renderer_material_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyMaterial(nu_renderer_material_handle_t handle)
{
    m_materials.erase(NU_HANDLE_GET_ID(handle));
}
const Material &AssetManager::getMaterial(nu_renderer_material_handle_t handle) const
{
    return *m_materials.at(NU_HANDLE_GET_ID(handle)).get();
}

nu_renderer_font_handle_t AssetManager::createFont(const nu_renderer_font_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_fonts.emplace(id, std::make_unique<Font>(info));
    nu_renderer_font_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyFont(nu_renderer_font_handle_t handle)
{
    m_fonts.erase(NU_HANDLE_GET_ID(handle));
}
const Font &AssetManager::getFont(nu_renderer_font_handle_t handle) const
{
    return *m_fonts.at(NU_HANDLE_GET_ID(handle)).get();
}