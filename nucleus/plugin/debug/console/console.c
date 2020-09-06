#include "console.h"

#define MAX_LINE_BUFFER_SIZE 32

typedef struct {
    nu_renderer_font_handle_t font;
    nu_renderer_label_handle_t label;
    char line_buffer[MAX_LINE_BUFFER_SIZE];
    uint32_t line_buffer_size;
} nu_console_data_t;

static nu_console_data_t _data;

nu_result_t nudebug_plugin_console_initialize(void)
{
    nu_renderer_font_create_info_t font_info;
    font_info.filename = "engine/font/Coder's Crux.ttf";
    if (nu_renderer_font_create(&_data.font, &font_info) != NU_SUCCESS) {
        nu_fatal("Failed to create font.\n");
    }

    nu_renderer_label_create_info_t label_info;
    label_info.x = -1;
    label_info.y = 50;
    label_info.font = _data.font;
    label_info.text = "";
    if (nu_renderer_label_create(&_data.label, &label_info) != NU_SUCCESS) {
        nu_fatal("Failed to create label.\n");
    }

    _data.line_buffer_size = 0;

    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_update(void)
{
    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);
    if (cursor_mode == NU_CURSOR_MODE_NORMAL) {

        /* append command line */
        const char *str;
        uint32_t str_len;
        nu_input_get_keyboard_text(&str, &str_len);
        if (str_len && (_data.line_buffer_size + str_len) < MAX_LINE_BUFFER_SIZE) {
            strcat(_data.line_buffer, str);
            _data.line_buffer_size += str_len;
            nu_renderer_label_set_text(_data.label, _data.line_buffer);
        }

        /* return key */
        nu_button_state_t backspace_state;
        nu_input_get_keyboard_state(&backspace_state, NU_KEYBOARD_BACKSPACE);
        if (backspace_state == NU_BUTTON_PRESSED) {
            if (_data.line_buffer_size > 0) {
                _data.line_buffer_size--;
                _data.line_buffer[_data.line_buffer_size] = '\0';
                nu_renderer_label_set_text(_data.label, _data.line_buffer);
            }
        }
    }

    return NU_SUCCESS;
}