#pragma once

extern "C"
{
    #include "../../../core/nucleus.h"
}

namespace nudebug
{
    class cursor_t
    {
    public:
        cursor_t();
        ~cursor_t();

        void set_position(uint32_t x, uint32_t y);
        void set_visible(bool visible);
        bool is_visible();

    private:
        nu_renderer_rectangle_handle_t m_handle;
        nu_rect_t m_rect;
    };
}