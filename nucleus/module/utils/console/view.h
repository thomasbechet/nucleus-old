#ifndef NUUTILS_CONSOLE_VIEW_H
#define NUUTILS_CONSOLE_VIEW_H

#include <nucleus/nucleus.h>

typedef struct {
    /* common */
    bool visible;
    nu_renderer_font_t font;
    nu_vec2i_t position;

    /* command buffer */
    nu_array_t command_buffer_labels;
    nu_array_t command_buffer_strings;
    uint32_t command_buffer_head;
    uint32_t command_buffer_tail;

    /* command */
    nu_renderer_label_t command_label;
    nu_string_t command_string;

    /* background */
    nu_renderer_rectangle_t background;

    /* cursor */
    nu_renderer_rectangle_t cursor;
    uint32_t cursor_character_position;
    float cursor_delta;
    float cursor_frequency;
} nuutils_console_view_t;

void nuutils_console_view_initialize(nuutils_console_view_t *view, nu_renderer_font_t font);
void nuutils_console_view_terminate(nuutils_console_view_t *view);
void nuutils_console_view_update(nuutils_console_view_t *view);
void nuutils_console_view_set_command(nuutils_console_view_t *view, const char *cstr);
void nuutils_console_view_add_old_command(nuutils_console_view_t *view, const char *cstr);
void nuutils_console_view_set_visible(nuutils_console_view_t *view, bool toggle);
void nuutils_console_view_set_cursor_position(nuutils_console_view_t *view, uint32_t position);
void nuutils_console_view_refresh(nuutils_console_view_t *view);

#endif