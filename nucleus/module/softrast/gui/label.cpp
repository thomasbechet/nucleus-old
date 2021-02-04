#include "label.hpp"

using namespace nu::softrast;

Label::Label(const nu_renderer_label_create_info_t &info)
{
    position = Vector2i(info.position);
    if (info.text) {
        text = std::string(info.text);
    }
    font = info.font;
}