#pragma once

#include <string>

extern "C"
{
    #include "../../../core/nucleus.h"
}

namespace nudebug
{
    class console_line_t
    {
    public:
        console_line_t(nu_renderer_font_handle_t font);
        ~console_line_t();

    private:
        std::string m_text;
        nu_renderer_label_handle_t m_handle;
    };
}