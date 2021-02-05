#pragma once

#include <nucleus/nucleus.hpp>

#include <string>

namespace nu::softrast
{
    struct Rectangle
    {
        Rectangle(const nu_renderer_rectangle_create_info_t &info);
        
        Rect rect;
        uint32_t color;
    };
}