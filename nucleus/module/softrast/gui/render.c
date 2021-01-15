#include "render.h"

nu_result_t nusr_gui_render_label(
    nusr_framebuffer_t *color_buffer,
    int32_t pos_x, int32_t pos_y,
    const nusr_font_t *font,
    const char *label
)
{
    uint32_t len = strlen(label);
    int32_t current_x = pos_x;

    /* iterate over characters */
    for (uint32_t ic = 0; ic < len; ic++) {
        const nusr_glyph_t *g;
        if (nusr_font_get_glyph(font, label[ic], &g) != NU_SUCCESS) continue;

        /* compute visibility bound */
        nu_rect_t window_bound;
        window_bound.left = 0;
        window_bound.top = 0;
        window_bound.width = color_buffer->width;
        window_bound.height = color_buffer->height;

        nu_rect_t character_bound;
        character_bound.left = 0;
        character_bound.top = 0;
        character_bound.width = g->bitmap_width;
        character_bound.height = g->bitmap_height;

        /* translate character bound */
        character_bound.left += (current_x + g->bearing_x);
        character_bound.top += (pos_y - g->bearing_y);

        /* clip and translate back character */
        nu_rect_clip(&character_bound, &window_bound);
        character_bound.left -= (current_x + g->bearing_x);
        character_bound.top -= (pos_y - g->bearing_y);

        /* draw character */
        const uint32_t start_x = character_bound.left;
        const uint32_t stop_x = start_x + character_bound.width;
        const uint32_t start_y = character_bound.top;
        const uint32_t stop_y = start_y + character_bound.height;
        for (uint32_t y = start_y; y < stop_y; y++) {
            for (uint32_t x = start_x; x < stop_x; x++) {
                const uint32_t x_buf = x + (current_x + g->bearing_x);
                const uint32_t y_buf = y + pos_y - g->bearing_y;

                uint32_t color = font->atlas[(g->ty + y) * font->width + x];
                nusr_framebuffer_blend_uint(color_buffer, x_buf, y_buf, color);
            }
        }

        /* goto next character */
        current_x += g->advance_x;
    }

    return NU_SUCCESS;
}
nu_result_t nusr_gui_render_rectangle(
    nusr_framebuffer_t *color_buffer,
    nu_rect_t rectangle,
    uint32_t color
)
{
    /* compute visibility bound */
    nu_rect_t window_bound;
    window_bound.left = 0;
    window_bound.top = 0;
    window_bound.width = color_buffer->width;
    window_bound.height = color_buffer->height;

    /* clip rectangle */
    nu_rect_clip(&rectangle, &window_bound);

    /* draw */
    for (uint32_t y = rectangle.top; y < rectangle.top + rectangle.height; y++) {
        for (uint32_t x = rectangle.left; x < rectangle.left + rectangle.width; x++) {
            nusr_framebuffer_blend_uint(color_buffer, x, y, color);
        }
    }

    return NU_SUCCESS;
}