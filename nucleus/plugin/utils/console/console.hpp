#pragma once

#include <string>
#include <vector>
#include <memory>

#include "command_line.hpp"
#include "cursor.hpp"

#include "../../../core/nucleus.h"

namespace nu::utility
{
    class console_t
    {
    public:
        console_t();
        ~console_t();

        void update();

    private:
        void update_cursor_advance();
        void set_command_line(std::string command);

        std::unique_ptr<command_line_t> m_command_line;
        std::vector<std::string> m_old_commands;
        uint32_t m_selected_old_command;
        std::unique_ptr<cursor_t> m_cursor;
        uint32_t m_selected_character;
        nu_renderer_font_handle_t m_font;
    };
}

nu_result_t nuutils_plugin_console_initialize(void);
nu_result_t nuutils_plugin_console_terminate(void);
nu_result_t nuutils_plugin_console_update(void);