#pragma once

#include "material.hpp"

#include <unordered_map>

namespace nurt
{
    class AssetManager
    {
    public:
        Material createMaterial(
            const nu_vec3f_t albedo
        );
        void destroyMaterial(MaterialHandle handle);
        Material getMaterial(MaterialHandle handle);

    private:
        std::unordered_map<MaterialHandle, Material> m_materials;
        uint32_t m_materialNextId = 0;
    };
}