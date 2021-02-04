#pragma once

#include <string>

#include <nucleus/nucleus.h>

namespace nu::utility
{
    class CommandLine
    {
    public:
        CommandLine(nu_renderer_font_handle_t font);
        ~CommandLine();

        void setPosition(const nu_vec2i_t position);
        void removeAt(uint32_t index);
        void appendAt(uint32_t index, const std::string &str);
        void clear();
        uint32_t size();
        void setCommand(std::string command);
        std::string getCommand();
        void setVisible(bool visible);

    private:
        void updateLabelPosition();
        void updateLabelText();

        std::string m_command;
        nu_vec2i_t m_position;
        bool m_visible;
        nu_renderer_label_handle_t m_handle;
    };
}