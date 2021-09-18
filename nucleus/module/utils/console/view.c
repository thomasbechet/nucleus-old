#include <nucleus/module/utils/console/view.h>

#define NUUTILS_LOGGER_NAME "CONSOLE"

#define COMMAND_BUFFER_MAX 10
#define FONT_SIZE 16

void nuutils_console_view_initialize(nuutils_console_view_t *view, nu_renderer_font_t font)
{
    nu_renderer_label_create_info_t label_info;
    nu_renderer_rectangle_create_info_t rectangle_info;

    /* set default label creation info */
    memset(&label_info, 0, sizeof(nu_renderer_label_create_info_t));
    label_info.position[0] = 0;
    label_info.position[1] = 0;
    label_info.font        = font;
    label_info.text        = "";

    /* common */
    view->visible     = true;
    view->font        = font;
    view->position[0] = 10;
    view->position[1] = 10;

    /* command buffer */
    view->command_buffer_head = 0;
    view->command_buffer_tail = 0;
    nu_array_allocate_capacity(&view->command_buffer_labels, sizeof(nu_renderer_label_t), COMMAND_BUFFER_MAX);
    nu_array_allocate_capacity(&view->command_buffer_strings, sizeof(nu_string_t), COMMAND_BUFFER_MAX);
    for (uint32_t i = 0; i < COMMAND_BUFFER_MAX; i++) {
        /* create labels */
        nu_renderer_label_t label;
        if (nu_renderer_label_create(&label_info, &label) != NU_SUCCESS) {
            nu_interrupt(NUUTILS_LOGGER_NAME, "Failed to create the label.");
        }
        nu_array_push(view->command_buffer_labels, &label);

        /* create strings */
        nu_string_t string;
        nu_string_allocate(&string);
        nu_array_push(view->command_buffer_strings, &string);
    }

    /* command */
    if (nu_renderer_label_create(&label_info, &view->command_label) != NU_SUCCESS) {
        nu_interrupt(NUUTILS_LOGGER_NAME, "Failed to create the label.");
    }
    nu_string_allocate(&view->command_string);

    /* background */
    memset(&rectangle_info, 0, sizeof(nu_renderer_rectangle_create_info_t));
    rectangle_info.color = 0xFFFFFF33;
    rectangle_info.rect = (nu_rect_t){0, 0, 0, 0};
    if (nu_renderer_rectangle_create(&rectangle_info, &view->background) != NU_SUCCESS) {
        nu_interrupt(NUUTILS_LOGGER_NAME, "Failed to create rectangle.");
    }

    /* cursor */
    view->cursor_delta              = 0.0;
    view->cursor_frequency          = 500.0;
    view->cursor_character_position = 0;
    memset(&rectangle_info, 0, sizeof(nu_renderer_rectangle_create_info_t));
    rectangle_info.color = 0xFFFFFFFF;
    if (nu_renderer_rectangle_create(&rectangle_info, &view->cursor) != NU_SUCCESS) {
        nu_interrupt(NUUTILS_LOGGER_NAME, "Failed to create rectangle.");
    }
}
void nuutils_console_view_terminate(nuutils_console_view_t *view)
{
    /* command buffer */
    for (uint32_t i = 0; i < COMMAND_BUFFER_MAX; i++) {
        nu_renderer_label_t *label = (nu_renderer_label_t*)nu_array_get(view->command_buffer_labels, i);
        nu_renderer_label_destroy(*label);
        nu_string_t *string = (nu_string_t*)nu_array_get(view->command_buffer_strings, i);
        nu_string_free(*string);
    }
    nu_array_free(view->command_buffer_labels);
    nu_array_free(view->command_buffer_strings);

    /* command */
    nu_renderer_label_destroy(view->command_label);
    nu_string_free(view->command_string);

    /* background */
    nu_renderer_rectangle_destroy(view->background);

    /* cursor */
    nu_renderer_rectangle_destroy(view->cursor);
}
void nuutils_console_view_update(nuutils_console_view_t *view)
{
       
}
void nuutils_console_view_set_command(nuutils_console_view_t *view, const char *cstr)
{
    nu_string_set_cstr(&view->command_string, cstr);   
}
void nuutils_console_view_add_old_command(nuutils_console_view_t *view, const char *cstr)
{
    view->command_buffer_head = (view->command_buffer_head + 1) % COMMAND_BUFFER_MAX;
    if (view->command_buffer_head == view->command_buffer_tail) {
        view->command_buffer_tail = (view->command_buffer_tail + 1) % COMMAND_BUFFER_MAX;
    } 
    nu_string_t *string = (nu_string_t*)nu_array_get(view->command_buffer_strings, view->command_buffer_head);
    nu_string_set_cstr(string, cstr);
}
void nuutils_console_view_set_visible(nuutils_console_view_t *view, bool toggle)
{
    view->visible = toggle;
}
void nuutils_console_view_set_cursor_position(nuutils_console_view_t *view, uint32_t position)
{
    view->cursor_character_position = position;
}
void nuutils_console_view_refresh(nuutils_console_view_t *view)
{
    /* get viewport size */
    nu_vec2u_t vsize;
    nu_renderer_viewport_get_size(vsize);

    /* get line height */
    nu_vec2u_t fsize;
    nu_renderer_font_get_text_size(view->font, "A", fsize);

    if (view->visible) {
        /* command buffer */
        nu_vec2i_t current_position;
        nu_vec2i_copy(view->position, current_position);
        for (uint32_t i = view->command_buffer_head;
             i != view->command_buffer_tail;
             i = (i + 1) % COMMAND_BUFFER_MAX) {
            nu_renderer_label_t *label = (nu_renderer_label_t*)nu_array_get(view->command_buffer_labels, i);
            /* update position */
            nu_renderer_label_set_position(*label, current_position);
            current_position[1] -= (fsize[1] + 1);
            /* update text */
            nu_string_t *string = (nu_string_t*)nu_array_get(view->command_buffer_strings, i);
            nu_renderer_label_set_text(*label, nu_string_get_cstr(*string));
        }

        /* command */
        // nu_string_insert_cstr(&view->command_string, ">", 0);
        // nu_renderer_label_set_text(view->command_label, nu_string_get_cstr(view->command_string));
        // nu_string_erase(&view->command_string, 0, 1);
        // nu_renderer_label_set_position(view->command_label, (nu_rect_t) {
        //     view->position[0
        // });

        /* background */
        // TODO:

        /* cursor */
        // TODO:
    } else {
        /* command buffer */
        for (uint32_t i = 0; i < COMMAND_BUFFER_MAX; i++) {
            nu_renderer_label_t *label = (nu_renderer_label_t*)nu_array_get(view->command_buffer_labels, i);
            nu_renderer_label_set_text(*label, "");
        }

        /* command */
        nu_renderer_label_set_text(view->command_label, "");

        /* background */
        nu_renderer_rectangle_set_rect(view->background,
            (nu_rect_t){0, 0, 0, 0});

        /* cursor */
        nu_renderer_rectangle_set_rect(view->cursor,
            (nu_rect_t){0, 0, 0, 0});
    }
}