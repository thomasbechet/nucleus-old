#pragma once

#include <string>

#include "../../../core/nucleus.h"

namespace nu::utility
{
    class command_line_t
    {
    public:
        command_line_t(nu_renderer_font_handle_t font);
        ~command_line_t();

        void set_position(uint32_t x, uint32_t y);
        void remove_at(uint32_t index);
        void append_at(uint32_t index, const std::string &str);
        void clear();
        uint32_t size();
        void set_command(std::string command);
        std::string get_command();
        void set_visible(bool visible);

    private:
        void update_label_position();
        void update_label_text();

        std::string m_command;
        uint32_t m_x;
        uint32_t m_y;
        bool m_visible;
        nu_renderer_label_handle_t m_handle;
    };
}