#pragma once

#include "../../../core/nucleus.h"

namespace nu::utility
{
    class cursor_t
    {
    public:
        cursor_t(float frequency);
        ~cursor_t();

        void set_position(uint32_t x, uint32_t y);
        void set_visible(bool visible);
        void set_advance(uint32_t advance);

        void update(float delta);

    private:
        void update_rectangle();

        nu_renderer_rectangle_handle_t m_handle;
        nu_rect_t m_rect;
        float m_delta;
        float m_frequency;
        bool m_visible;
        uint32_t m_advance;
    };
}