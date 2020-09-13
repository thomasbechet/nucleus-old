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

static void update_cursor_position(void)
{
    uint32_t w, h, vw, vh;
    nu_renderer_viewport_get_size(&vw, &vh);
    nu_renderer_font_get_text_size(_data.font, _data.line_buffer, &w, &h);
    _data.cursor_rect.left = (FONT_SIZE / 2) + w;
    nu_renderer_rectangle_set_rect(_data.cursor_rectangle, _data.cursor_rect);
}
static void swap_cursor_visibility(void)
{
    _data.cursor_rect.height = (_data.cursor_rect.height > 0) ? 0 : 1;
    nu_renderer_rectangle_set_rect(_data.cursor_rectangle, _data.cursor_rect);
}
static void hide_cursor(void)
{
    _data.cursor_rect.height = 0;
    nu_renderer_rectangle_set_rect(_data.cursor_rectangle, _data.cursor_rect);
}

nu_result_t nudebug_plugin_console_initialize(void)
{
    _data.console = std::make_unique<console_t>();
    return NU_SUCCESS;

    /* recover viewport size */
    uint32_t width, height;
    nu_renderer_viewport_get_size(&width, &height);

    /* create font */
    nu_renderer_font_create_info_t font_info;
    font_info.filename = "engine/font/Coder's Crux.ttf";
    font_info.font_size = FONT_SIZE;
    if (nu_renderer_font_create(&_data.font, &font_info) != NU_SUCCESS) {
        nu_fatal(NUDEBUG_LOGGER_NAME"Failed to create font.\n");
    }

    /* create command line label */
    nu_renderer_label_create_info_t label_info;
    label_info.x = FONT_SIZE / 2;
    label_info.y = height - FONT_SIZE / 2;
    label_info.font = _data.font;
    label_info.text = "";
    if (nu_renderer_label_create(&_data.command_label, &label_info) != NU_SUCCESS) {
        nu_fatal(NUDEBUG_LOGGER_NAME"Failed to create command label.\n");
    }

    /* create cursor rectangle */
    nu_renderer_rectangle_create_info_t rectangle_info;
    _data.cursor_rect.top = height - FONT_SIZE / 2;
    _data.cursor_rect.left = FONT_SIZE / 2;
    _data.cursor_rect.width = CURSOR_WIDTH;
    _data.cursor_rect.height = 1;
    rectangle_info.rect = _data.cursor_rect;
    rectangle_info.color = 0xFFFFFFFF;
    if (nu_renderer_rectangle_create(&_data.cursor_rectangle, &rectangle_info) != NU_SUCCESS) {
        nu_fatal(NUDEBUG_LOGGER_NAME"Failed to create cursor rectangle.\n");
    }

    /* initialize text */
    _data.line_buffer_size = 0;
    nu_renderer_label_set_text(_data.command_label, "");

    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_terminate(void)
{
    _data.console.reset();
    return NU_SUCCESS;

    /* destroy label */
    nu_renderer_label_destroy(_data.command_label);
    
    /* destroy rectangle */
    nu_renderer_rectangle_destroy(_data.cursor_rectangle);

    /* destroy font */
    nu_renderer_font_destroy(_data.font);

    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_update(void)
{
    _data.console->update();
    return NU_SUCCESS;

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
        if (str_len && (_data.line_buffer_size + str_len) < MAX_LINE_BUFFER_SIZE) {
            strcat(_data.line_buffer, str);
            _data.line_buffer_size += str_len;
            nu_renderer_label_set_text(_data.command_label, _data.line_buffer);

            update_cursor_position();
        }

        /* backspace key */
        nu_button_state_t backspace_state;
        nu_input_get_keyboard_state(&backspace_state, NU_KEYBOARD_BACKSPACE);
        if (backspace_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (_data.line_buffer_size > 0) {
                _data.line_buffer_size--;
                _data.line_buffer[_data.line_buffer_size] = '\0';
                nu_renderer_label_set_text(_data.command_label, _data.line_buffer);

                update_cursor_position();
            }
        }

        /* enter key */
        nu_button_state_t enter_state;
        nu_input_get_keyboard_state(&enter_state, NU_KEYBOARD_ENTER);
        if (enter_state & NU_BUTTON_JUST_PRESSED) {
            _data.line_buffer_size = 0;
            _data.line_buffer[0] = '\0';
            nu_renderer_label_set_text(_data.command_label, "");
        }
    }

    return NU_SUCCESS;
}