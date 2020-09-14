#include "command_line.hpp"

#include "../common/logger.hpp"

using namespace nudebug;

command_line_t::command_line_t(nu_renderer_font_handle_t font)
{
    m_x = 0;
    m_y = 0;

    /* create label */
    nu_renderer_label_create_info_t label_info;
    label_info.x = m_x;
    label_info.y = m_y;
    label_info.font = font;
    label_info.text = "";
    if (nu_renderer_label_create(&m_handle, &label_info) != NU_SUCCESS) {
        nu_fatal(NUDEBUG_LOGGER_NAME"Failed to create command label.\n");
    }
}
command_line_t::~command_line_t()
{
    /* destroy label */
    nu_renderer_label_destroy(m_handle);
}

void command_line_t::set_position(uint32_t x, uint32_t y)
{
    m_x = x;
    m_y = y;
    update_label_position();
}
void command_line_t::remove_at(uint32_t index)
{
    m_command.erase(m_command.begin() + index);
    update_label_text();
}
void command_line_t::append_at(uint32_t index, const std::string &str)
{
    m_command.insert(index, str);
    update_label_text();
}
void command_line_t::clear()
{
    m_command.clear();
    update_label_text();
}
uint32_t command_line_t::size()
{
    return m_command.size();
}
std::string command_line_t::get_command()
{
    return m_command;
}

void command_line_t::update_label_position()
{
    nu_renderer_label_set_position(m_handle, m_x, m_y);
}
void command_line_t::update_label_text()
{
    nu_renderer_label_set_text(m_handle, m_command.c_str());
}