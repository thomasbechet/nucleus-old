#pragma once


#include <string>
#include <vector>

#include "console_line.hpp"
#include "cursor.hpp"

extern "C"
{
    #include "../../../core/nucleus.h"
}

nu_result_t nudebug_plugin_console_initialize(void);
nu_result_t nudebug_plugin_console_terminate(void);
nu_result_t nudebug_plugin_console_update(void);

namespace nudebug
{
    class console_t
    {
    public:
        console_t();
        ~console_t();

        void update() noexcept;

    private:
        console_line_t m_line;
        std::vector<console_line_t> m_old_lines;
        cursor_t m_cursor;
        nu_renderer_font_handle_t m_font;
    };
}