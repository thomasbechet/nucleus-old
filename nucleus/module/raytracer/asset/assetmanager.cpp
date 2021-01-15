#include "assetmanager.hpp"

using namespace nurt;

Material AssetManager::createMaterial(
    const nu_vec3f_t albedo
)
{
    Material material;
    NU_HANDLE_SET_ID(material.handle, m_materialNextId++);
    nu_vec3f_copy(albedo, material.albedo);
    m_materials.emplace(material.handle, material);
    return material;
}
void AssetManager::destroyMaterial(MaterialHandle handle)
{
    m_materials.erase(handle);
}
Material AssetManager::getMaterial(MaterialHandle handle)
{
    return m_materials.at(handle);
}