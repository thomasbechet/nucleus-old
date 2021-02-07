#pragma once

#include "line.hpp"

#include <vector>
#include <memory>

namespace nu::utility
{
    class LineBuffer
    {
    public:
        LineBuffer(nu_renderer_font_handle_t font, uint32_t maxLine);

        void add(const std::string &text);
        void clear();
        void setPosition(const Vector2i &position);
        void setVisible(bool toggle);

    private:
        void updateLinesPosition();

        Vector2i m_position;
        std::vector<std::unique_ptr<Line>> m_lines;
        nu_renderer_font_handle_t m_font;
        bool m_visible = true;
        uint32_t m_labelHeight;
        uint32_t m_maxLine;
    };
}