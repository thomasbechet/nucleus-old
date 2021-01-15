#pragma once

#include <string>
#include <vector>
#include <memory>

#include "command_line.hpp"
#include "cursor.hpp"

#include <nucleus/nucleus.h>

nu_result_t nuutils_console_plugin_initialize(void);
nu_result_t nuutils_console_plugin_terminate(void);
nu_result_t nuutils_console_plugin_update(void);

namespace nu::utility
{
    class console_t
    {
    public:
        console_t();
        ~console_t();

        void update();
        nu_result_t on_event(nu_event_id_t id, void *data);

    private:
        void update_cursor_advance();
        void set_command_line(std::string command);
        void update_position();

        std::unique_ptr<command_line_t> m_command_line;
        std::vector<std::string> m_old_commands;
        uint32_t m_selected_old_command;
        std::unique_ptr<cursor_t> m_cursor;
        uint32_t m_selected_character;
        nu_renderer_font_handle_t m_font;
    };
}