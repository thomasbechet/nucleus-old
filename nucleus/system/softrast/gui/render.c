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
        const nusr_glyph_t *g = &font->glyphs[(uint32_t)label[ic] - 32];

        /* compute visibility bound */
        const int32_t left_border = 0;
        const int32_t top_border = 0;

        const int32_t pixel_x = current_x + (int32_t)g->bearing_x;
        const int32_t pixel_y = pos_y - (int32_t)g->bearing_y;

        uint32_t start_x, start_y;
        if ((left_border - pixel_x) > left_border) {

        } else {

        }

        uint32_t start_x = (left_border - pixel_x) > left_border ? pixel_x : left_border;
        uint32_t start_y = (top_border - pixel_y) > top_border ? pixel_y : top_border;

        nu_info("START X %d\n", start_x);
        nu_info("START Y %d\n", start_y);

        uint32_t pix_start_x = pixel_x < 0 ? 0 - pixel_x : 0 /* start texture */;
        uint32_t pix_start_y = pixel_y < 0 ? 0 - pixel_y : 0 /* start texture */;

        /* draw character */
        for (uint32_t y = pix_start_y; y < pix_stop_y; y++) {
            for (uint32_t x = pix_start_x; x < pix_stop_x; x++) {
                uint32_t x_tex = x - pixel_x;
                uint32_t y_tex = y - pixel_y;
                uint32_t color = font->atlas[(g->ty + y_tex) * font->width + x_tex];
                nusr_framebuffer_blend_uint(color_buffer, x, y, color);
            }
        }

        // for (uint32_t y = 0; y < g->bitmap_height; y++) {
        //     for (uint32_t x = 0; x < g->bitmap_width; x++) {
        //         uint32_t x_buf = current_x + g->bearing_x + x;
        //         uint32_t y_buf = pos_y - g->bearing_y + y;
        //         uint32_t color = font->atlas[(g->ty + y) * font->width + x];
        //         nusr_framebuffer_blend_uint(color_buffer, x_buf, y_buf, color);
        //     }
        // }

        /* goto next character */
        current_x += g->advance_x;
    }

    return NU_SUCCESS;
}