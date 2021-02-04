#include "commandline.hpp"

#include "logger.hpp"

using namespace nu::utility;

CommandLine::CommandLine(nu_renderer_font_handle_t font)
{
    nu_vec2i_zero(m_position);

    /* create label */
    nu_renderer_label_create_info_t label_info;
    nu_vec2i_copy(m_position, label_info.position);
    label_info.font = font;
    label_info.text = "";
    if (nu_renderer_label_create(&m_handle, &label_info) != NU_SUCCESS) {
        nu_fatal(NUUTILS_LOGGER_NAME"Failed to create command label.\n");
    }
}
CommandLine::~CommandLine()
{
    // Destroy label
    nu_renderer_label_destroy(m_handle);
}

void CommandLine::setPosition(const nu_vec2i_t position)
{
    nu_vec2i_copy(position, m_position);
    updateLabelPosition();
}
void CommandLine::removeAt(uint32_t index)
{
    m_command.erase(m_command.begin() + index);
    updateLabelText();
}
void CommandLine::appendAt(uint32_t index, const std::string &str)
{
    m_command.insert(index, str);
    updateLabelText();
}
void CommandLine::clear()
{
    m_command.clear();
    updateLabelText();
}
uint32_t CommandLine::size()
{
    return m_command.size();
}
void CommandLine::setCommand(std::string command)
{
    m_command = command;
    updateLabelText();
}
std::string CommandLine::getCommand()
{
    return m_command;
}
void CommandLine::setVisible(bool visible)
{
    m_visible = visible;
    updateLabelText();
}

void CommandLine::updateLabelPosition()
{
    nu_renderer_label_set_position(m_handle, m_position);
}
void CommandLine::updateLabelText()
{
    if (m_visible) {
        nu_renderer_label_set_text(m_handle, m_command.c_str());
    } else {
        nu_renderer_label_set_text(m_handle, "");
    }
}