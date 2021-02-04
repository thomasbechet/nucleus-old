#pragma once

#include "../module/interface.h"
#include "../memory/colorframebuffer.hpp"

namespace nu::softrast
{
    struct Texture : public ColorFramebuffer
    {
        Texture(const nu_renderer_texture_create_info_t &info);
    };
}