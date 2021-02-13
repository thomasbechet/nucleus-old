#pragma once

#include <nucleus/module/utils/console/line.hpp>

namespace nu::utility
{
    class CommandLine : public Line
    {
    public:
        CommandLine(nu_renderer_font_handle_t font);

        void removeAt(uint32_t index);
        void appendAt(uint32_t index, const std::string &str);
        void clear();
        uint32_t size();
        void setCommand(std::string command);
        std::string getCommand();

    private:
        void updateText();

        std::string m_command;
    };
}