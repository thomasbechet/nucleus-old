#pragma once

#include <nucleus/nucleus.h>

namespace nusr
{
    struct Material
    {
        Material(const nu_renderer_material_create_info_t &info);

        nu_renderer_texture_handle_t texture;
    };
}