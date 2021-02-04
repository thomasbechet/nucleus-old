#pragma once

#include "../memory/colorframebuffer.hpp"
#include "../asset/assetmanager.hpp"
#include "label.hpp"

#include <unordered_map>

namespace nu::softrast
{
    class GUI
    {
    public:
        nu_renderer_label_handle_t createLabel(const nu_renderer_label_create_info_t &info);
        void destroyLabel(nu_renderer_label_handle_t handle);
        void setLabelPosition(nu_renderer_label_handle_t handle, const Vector2i &position);
        void setLabelText(nu_renderer_label_handle_t handle, const std::string &text);

        void draw(ColorFramebuffer &colorBuffer, const AssetManager &assetManager) const;

    private:
        void renderLabel(ColorFramebuffer &colorBuffer, const Font &font, const Vector2i &position, const std::string &text) const;

        std::unordered_map<uint32_t, Label> m_labels;
        uint32_t m_nextId = 0;
    };
}