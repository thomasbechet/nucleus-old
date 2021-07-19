#pragma once

#include <nucleus/nucleus.hpp>

namespace nu::softrast
{
    struct Material
    {
        Material(const nu_renderer_material_create_info_t &info);

        nu_renderer_texture_t texture;
    };
}