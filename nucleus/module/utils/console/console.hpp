#pragma once

#include <string>
#include <vector>
#include <memory>

#include "commandline.hpp"
#include "cursor.hpp"

#include <nucleus/nucleus.h>

nu_result_t nuutils_console_plugin_initialize(void);
nu_result_t nuutils_console_plugin_terminate(void);
nu_result_t nuutils_console_plugin_update(void);

namespace nu::utility
{
    class Console
    {
    public:
        Console();
        ~Console();

        void update();
        nu_result_t onEvent(nu_event_id_t id, void *data);

    private:
        void updateCursorAdvance();
        void setCommandLine(std::string command);
        void updatePosition();

        std::unique_ptr<CommandLine> m_commandLine;
        std::vector<std::string> m_oldCommands;
        uint32_t m_selectedOldCommand;
        std::unique_ptr<Cursor> m_cursor;
        uint32_t m_selectedCharacter;
        nu_renderer_font_handle_t m_font;
    };
}