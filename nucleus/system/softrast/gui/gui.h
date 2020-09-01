#ifndef NUSR_GUI_H
#define NUSR_GUI_H

#include "../memory/framebuffer.h"
#include "../asset/font.h"

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t font;
    const char *text;
} nusr_label_t;

NU_API nu_result_t nusr_gui_initialize(void);
NU_API nu_result_t nusr_gui_terminate(void);
NU_API nu_result_t nusr_gui_render(nusr_framebuffer_t *color_buffer);

#endif