#pragma once

#include <nucleus/nucleus.hpp>

#include <string>

namespace nu::softrast
{
    struct Label
    {
        Label(const nu_renderer_label_create_info_t &info);

        nu_renderer_font_t font;
        std::string text;
        Vector2i position;
    };
}