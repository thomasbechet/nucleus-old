#include "commandline.hpp"

using namespace nu::utility;

CommandLine::CommandLine(nu_renderer_font_handle_t font) : Line(font) 
{
    updateText();
}

void CommandLine::removeAt(uint32_t index)
{
    m_command.erase(m_command.begin() + index);
    updateText();
}
void CommandLine::appendAt(uint32_t index, const std::string &str)
{
    m_command.insert(index, str);
    updateText();
}
void CommandLine::clear()
{
    m_command.clear();
    updateText();
}
uint32_t CommandLine::size()
{
    return m_command.size();
}
void CommandLine::setCommand(std::string command)
{
    m_command = command;
    updateText();
}
std::string CommandLine::getCommand()
{
    return m_command;
}

void CommandLine::updateText()
{
    setText(">" + m_command);
}