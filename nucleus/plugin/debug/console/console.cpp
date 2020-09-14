#include "console.hpp"

#include "../common/logger.hpp"

#define MAX_LINE_BUFFER_SIZE 512
#define FONT_SIZE 16
#define CURSOR_WIDTH 5

#include <memory>

using namespace nudebug;

typedef struct {
    nu_renderer_font_handle_t font;

    nu_renderer_label_handle_t command_label;
    char line_buffer[MAX_LINE_BUFFER_SIZE];
    uint32_t line_buffer_size;

    nu_renderer_rectangle_handle_t cursor_rectangle;
    nu_rect_t cursor_rect;
    float cursor_delta;
    
    std::unique_ptr<console_t> console;
} nu_console_data_t;

static nu_console_data_t _data;

console_t::console_t()
{
    /* create font */
    nu_renderer_font_create_info_t font_info;
    font_info.filename = "engine/font/Coder's Crux.ttf";
    font_info.font_size = FONT_SIZE;
    if (nu_renderer_font_create(&_data.font, &font_info) != NU_SUCCESS) {
        nu_fatal(NUDEBUG_LOGGER_NAME"Failed to create font.\n");
    }

    /* create cursor */
    m_cursor = std::make_unique<cursor_t>(1.0f);
    m_selected_character = 0;

    /* create command */
    m_command_line = std::make_unique<command_line_t>(m_font);

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
        } else {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_NORMAL);
        }
    }

    if (cursor_mode == NU_CURSOR_MODE_NORMAL) {
        /* append command line */
        const char *str;
        uint32_t str_len;
        nu_input_get_keyboard_text(&str, &str_len);
        if (str_len) {
            m_command_line->append_at(m_selected_character, std::string(str));
            if (m_selected_character == m_command_line->size()) {
                m_selected_character++;
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
                }
                update_cursor_advance();
            }
        }

        /* arrow keys */
        nu_button_state_t up_state, down_state, left_state, right_state;
        nu_input_get_keyboard_state(&up_state, NU_KEYBOARD_UP);
        nu_input_get_keyboard_state(&down_state, NU_KEYBOARD_DOWN);
        nu_input_get_keyboard_state(&left_state, NU_KEYBOARD_LEFT);
        nu_input_get_keyboard_state(&right_state, NU_KEYBOARD_RIGHT);
        if (up_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            
        } else if (down_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {

        } else if (left_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selected_character > 0) {
                m_selected_character--;
                update_cursor_advance();
            }
        } else if (right_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selected_character <= m_command_line->size()) {
                m_selected_character++;
                update_cursor_advance();
            }
        }

        /* enter key */
        nu_button_state_t enter_state;
        nu_input_get_keyboard_state(&enter_state, NU_KEYBOARD_ENTER);
        if (enter_state & NU_BUTTON_JUST_PRESSED) {
            m_command_line->clear();
            update_cursor_advance();
        }
    }
}

void console_t::update_cursor_advance()
{
    uint32_t width, height;
    std::string sub_string = m_command_line->get_command().substr(0, m_selected_character);
    nu_renderer_font_get_text_size(m_font, sub_string.c_str(), &width, &height);
    m_cursor->set_advance(width);
}

nu_result_t nudebug_plugin_console_initialize(void)
{
    _data.console = std::make_unique<console_t>();
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_terminate(void)
{
    _data.console.reset();
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_update(void)
{
    _data.console->update();
    return NU_SUCCESS;
}