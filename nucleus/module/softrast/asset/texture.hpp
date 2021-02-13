#pragma once

#include <nucleus/module/softrast/module/interface.h>
#include <nucleus/module/softrast/memory/colorframebuffer.hpp>

namespace nu::softrast
{
    struct Texture : public ColorFramebuffer
    {
        Texture(const nu_renderer_texture_create_info_t &info);
    };
}