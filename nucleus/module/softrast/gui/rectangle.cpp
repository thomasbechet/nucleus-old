#include <nucleus/module/softrast/gui/rectangle.hpp>

using namespace nu;
using namespace nu::softrast;

Rectangle::Rectangle(const nu_renderer_rectangle_create_info_t &info)
{
    color = info.color;
    rect = Rect(info.rect);
}