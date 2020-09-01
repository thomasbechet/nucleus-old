#include "render.h"

nu_result_t nusr_gui_render_label(
    nusr_framebuffer_t *color_buffer,
    uint32_t pos_x, uint32_t pos_y,
    const nusr_font_t *font,
    const char *label
)
{
    uint32_t len = strlen(label);
    uint32_t current_x = pos_x;
    for (uint32_t ic = 0; ic < len; ic++) {
        const nusr_glyph_t *g = &font->glyphs[(uint32_t)label[ic] - 32];

        for (uint32_t y = 0; y < g->bitmap_height; y++) {
            for (uint32_t x = 0; x < g->bitmap_width; x++) {
                uint32_t x_buf = current_x + g->bearing_x + x;
                uint32_t y_buf = pos_y - g->bearing_y + y;
                uint32_t color = font->atlas[(g->ty + y) * font->width + x];
                nusr_framebuffer_blend_uint(color_buffer, x_buf, y_buf, color);
            }
        }

        current_x += g->advance_x;
    }

    return NU_SUCCESS;
}