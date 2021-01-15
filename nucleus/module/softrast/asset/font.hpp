#pragma once

#include "../module/interface.h"
#include "../memory/colorframebuffer.hpp"

#include <unordered_map>
#include <vector>

namespace nusr
{
    struct Glyph
    {
        uint32_t advance_x;
        uint32_t advance_y;

        uint32_t bitmap_width;
        uint32_t bitmap_height;

        uint32_t bearing_x;
        uint32_t bearing_y;

        uint32_t ty;
    };

    struct Font
    {
        Font(const nu_renderer_font_create_info_t &info);

        std::unordered_map<char, Glyph> glyphs;
        ColorFramebuffer atlas;

        void getTextSize(const std::string &text, uint32_t &width, uint32_t &height) const;
    };
}