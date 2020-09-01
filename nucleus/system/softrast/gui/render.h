#ifndef NUSR_GUI_RENDER_H
#define NUSR_GUI_RENDER_H

#include "gui.h"

NU_API nu_result_t nusr_gui_render_label(
    nusr_framebuffer_t *color_buffer,
    uint32_t x, uint32_t y,
    const nusr_font_t *font,
    const char *label
);

#endif