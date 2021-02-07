#pragma once

#include <nucleus/nucleus.hpp>

namespace nu::utility
{
    class Cursor
    {
    public:
        Cursor(float frequency);
        ~Cursor();

        void setPosition(const Vector2i &position);
        void setVisible(bool visible);
        void setAdvance(uint32_t advance);

        void update(float delta);

    private:
        void updateRectangle();

        nu_renderer_rectangle_handle_t m_handle;
        Rect m_rect;
        float m_delta;
        float m_frequency;
        bool m_visible;
        uint32_t m_advance;
    };
}