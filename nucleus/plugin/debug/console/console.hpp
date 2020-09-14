#pragma once

#include <string>
#include <vector>
#include <memory>

#include "command_line.hpp"
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

        void update();

    private:
        std::unique_ptr<command_line_t> m_command_line;
        std::unique_ptr<cursor_t> m_cursor;
        nu_renderer_font_handle_t m_font;
    };
}