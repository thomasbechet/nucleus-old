#pragma once

#include "../../../core/nucleus.h"
#include "texture.hpp"

#include <unordered_map>
#include <memory>

namespace nugl
{
    class material_t
    {
    public:
        material_t(const nu_renderer_material_create_info_t &info);

        static void bind(const material_t &material, const std::unordered_map<uint32_t, std::unique_ptr<texture_t>> &textures);

    private:
        uint32_t m_diffuse_texture;
        bool m_use_diffuse_uniform;
    };
}