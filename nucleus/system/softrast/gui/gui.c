#include "gui.h"

#include "render.h"

typedef struct {
    char buf[1024];
} nusr_gui_data_t;

static nusr_gui_data_t _data;

nu_result_t nusr_gui_initialize(void)
{
    memset(_data.buf, 0, sizeof(char) * 1024);

    return NU_SUCCESS;    
}
nu_result_t nusr_gui_terminate(void)
{


    return NU_SUCCESS;
}
nu_result_t nusr_gui_render(nusr_framebuffer_t *color_buffer)
{
    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);
    if (cursor_mode == NU_CURSOR_MODE_NORMAL) {
        const char *str;
        uint32_t str_len;
        nu_input_get_keyboard_text(&str, &str_len);
        if (str_len) strcat(_data.buf, str);
    }
    
    nusr_font_t *font;
    nusr_font_get(0, &font);

    nusr_gui_render_label(color_buffer, 3, 360 - 3, font, _data.buf);

    return NU_SUCCESS;
}