#include "console.hpp"

#include "../common/logger.hpp"

#include <memory>

#define FONT_SIZE 16

using namespace nu::utility;

typedef struct {    
    std::unique_ptr<console_t> console;
} nu_console_data_t;

static nu_console_data_t _data;

console_t::console_t()
{
    /* create font */
    nu_renderer_font_create_info_t font_info;
    font_info.filename = "engine/font/Coder's Crux.ttf";
    font_info.font_size = FONT_SIZE;
    if (nu_renderer_font_create(&m_font, &font_info) != NU_SUCCESS) {
        nu_fatal(NU_UTILITY_LOGGER_NAME"Failed to create font.\n");
    }

    /* create cursor */
    m_cursor = std::make_unique<cursor_t>(500.0f);
    m_cursor->set_visible(false);
    m_selected_character = 0;

    /* create command */
    m_command_line = std::make_unique<command_line_t>(m_font);
    m_selected_old_command = 0;

    /* set component position */
    uint32_t w, h, vw, vh;
    nu_renderer_viewport_get_size(&vw, &vh);
    m_cursor->set_position(FONT_SIZE / 2, vh - FONT_SIZE / 2);
    m_command_line->set_position(FONT_SIZE / 2, vh - FONT_SIZE / 2);
}
console_t::~console_t()
{
    /* destroy command line */
    m_command_line.reset();

    /* destroy cursor */
    m_cursor.reset();

    /* destroy font */
    nu_renderer_font_destroy(m_font);
}

void console_t::update()
{
    /* get cursor mode */
    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);

    /* enable or disable console */
    nu_button_state_t button_state;
    nu_input_get_keyboard_state(&button_state, NU_KEYBOARD_F1);
    if (button_state & NU_BUTTON_JUST_PRESSED) {
        if (cursor_mode == NU_CURSOR_MODE_NORMAL) {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_DISABLE);
            m_cursor->set_visible(false);
            m_command_line->set_visible(false);
        } else {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_NORMAL);
            m_cursor->set_visible(true);
            m_command_line->set_visible(true);
        }
    }

    if (cursor_mode == NU_CURSOR_MODE_NORMAL) {

        /* append command line */
        const char *str;
        uint32_t str_len;
        nu_input_get_keyboard_text(&str, &str_len);
        if (str_len) {
            std::string append_text(str);
            uint32_t command_size = m_command_line->size();
            m_command_line->append_at(m_selected_character, append_text);
            if (m_selected_character == command_size) {
                m_selected_character = m_command_line->size();
            } else {
                m_selected_character += append_text.size();
            }
            update_cursor_advance();
        }

        /* backspace key */
        nu_button_state_t backspace_state;
        nu_input_get_keyboard_state(&backspace_state, NU_KEYBOARD_BACKSPACE);
        if (backspace_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_command_line->size() > 0) {
                if (m_selected_character > 0) {
                    m_selected_character--;
                    m_command_line->remove_at(m_selected_character);
                    update_cursor_advance();
                }
            }
        }

        /* arrow keys */
        nu_button_state_t up_state, down_state, left_state, right_state;
        nu_input_get_keyboard_state(&up_state, NU_KEYBOARD_UP);
        nu_input_get_keyboard_state(&down_state, NU_KEYBOARD_DOWN);
        nu_input_get_keyboard_state(&left_state, NU_KEYBOARD_LEFT);
        nu_input_get_keyboard_state(&right_state, NU_KEYBOARD_RIGHT);
        if (up_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selected_old_command > 0) {
                m_selected_old_command--;
                set_command_line(m_old_commands.at(m_selected_old_command));
            }
        } else if (down_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selected_old_command < m_old_commands.size()) {
                m_selected_old_command++;
                if (m_selected_old_command == m_old_commands.size()) {
                    set_command_line("");
                } else {
                    set_command_line(m_old_commands.at(m_selected_old_command));
                }
            } else {
                set_command_line("");
            }
        } else if (left_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selected_character > 0) {
                m_selected_character--;
                update_cursor_advance();
            }
        } else if (right_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selected_character < m_command_line->size()) {
                m_selected_character++;
                update_cursor_advance();
            }
        }

        /* enter key */
        nu_button_state_t enter_state;
        nu_input_get_keyboard_state(&enter_state, NU_KEYBOARD_ENTER);
        if (enter_state & NU_BUTTON_JUST_PRESSED) {
            if (m_command_line->size() > 0) {
                /* add command to old commands */
                m_old_commands.push_back(m_command_line->get_command());
                m_selected_old_command = m_old_commands.size();
                
                /* clear command line */
                set_command_line("");
            }
        }

        /* udpate cursor */
        m_cursor->update(nu_context_get_delta_time());
    }
}

void console_t::update_cursor_advance()
{
    uint32_t width, height;
    std::string sub_string = m_command_line->get_command().substr(0, m_selected_character);
    nu_renderer_font_get_text_size(m_font, sub_string.c_str(), &width, &height);
    m_cursor->set_advance(width);
}
void console_t::set_command_line(std::string command)
{
    m_command_line->set_command(command);
    m_selected_character = command.size();
    update_cursor_advance();
}

nu_result_t nu_utility_plugin_console_initialize(void)
{
    _data.console = std::make_unique<console_t>();
    return NU_SUCCESS;
}
nu_result_t nu_utility_plugin_console_terminate(void)
{
    _data.console.reset();
    return NU_SUCCESS;
}
nu_result_t nu_utility_plugin_console_update(void)
{
    _data.console->update();
    return NU_SUCCESS;
}