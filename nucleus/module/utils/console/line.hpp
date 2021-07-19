#pragma once

#include <nucleus/nucleus.hpp>

#include <string>

namespace nu::utility
{
    class Line
    {
    public:
        Line(nu_renderer_font_t font);
        ~Line();

        void setPosition(const Vector2i &position);
        void setText(const std::string &text);
        void setVisible(bool toggle);
        std::string getText() const;

    private:
        void updateLabelText();

    protected:
        nu_renderer_label_t m_handle;
        std::string m_text;
        Vector2i m_position;
        bool m_visible = true;
    };
}