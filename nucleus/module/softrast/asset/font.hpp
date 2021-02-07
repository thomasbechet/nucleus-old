#pragma once

#include "../memory/colorframebuffer.hpp"

#include <unordered_map>
#include <vector>

namespace nu::softrast
{
    struct Glyph
    {
        Vector2u advance;
        Vector2u bitmapSize;
        Vector2u bearing;

        uint32_t ty;
    };

    struct Font
    {
        Font(const nu_renderer_font_create_info_t &info);

        std::unordered_map<char, Glyph> glyphs;
        ColorFramebuffer atlas;
        uint32_t lineHeight;

        Vector2u getTextSize(const std::string &text) const;
    };
}