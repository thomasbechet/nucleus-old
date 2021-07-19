#include <nucleus/module/softrast/asset/assetmanager.hpp>

using namespace nu::softrast;

nu_renderer_mesh_t AssetManager::createMesh(const nu_renderer_mesh_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_meshes.emplace(id, std::make_unique<Mesh>(info));
    nu_renderer_mesh_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyMesh(nu_renderer_mesh_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_meshes.erase(id);
}
const Mesh &AssetManager::getMesh(nu_renderer_mesh_t handle) const
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    return *m_meshes.at(id).get();
}

nu_renderer_texture_t AssetManager::createTexture(const nu_renderer_texture_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_textures.emplace(id, std::make_unique<Texture>(info));
    nu_renderer_texture_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyTexture(nu_renderer_texture_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_textures.erase(id);
}
const Texture &AssetManager::getTexture(nu_renderer_texture_t handle) const
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    return *m_textures.at(id).get();
}

nu_renderer_material_t AssetManager::createMaterial(const nu_renderer_material_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_materials.emplace(id, std::make_unique<Material>(info));
    nu_renderer_material_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyMaterial(nu_renderer_material_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_materials.erase(id);
}
const Material &AssetManager::getMaterial(nu_renderer_material_t handle) const
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    return *m_materials.at(id).get();
}

nu_renderer_font_t AssetManager::createFont(const nu_renderer_font_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_fonts.emplace(id, std::make_unique<Font>(info));
    nu_renderer_font_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void AssetManager::destroyFont(nu_renderer_font_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_fonts.erase(id);
}
const Font &AssetManager::getFont(nu_renderer_font_t handle) const
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    return *m_fonts.at(id).get();
}