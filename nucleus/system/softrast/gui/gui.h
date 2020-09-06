#ifndef NUSR_GUI_H
#define NUSR_GUI_H

#include "../memory/framebuffer.h"
#include "../asset/font.h"

#define NUSR_MAX_LABEL_TEXT_SIZE 512

typedef struct {
    int32_t x;
    int32_t y;
    uint32_t font;
    char text[NUSR_MAX_LABEL_TEXT_SIZE];
    bool active;
} nusr_label_t;

NU_API nu_result_t nusr_gui_initialize(void);
NU_API nu_result_t nusr_gui_terminate(void);
NU_API nu_result_t nusr_gui_render(nusr_framebuffer_t *color_buffer);

NU_API nu_result_t nusr_gui_label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info);
NU_API nu_result_t nusr_gui_label_destroy(nu_renderer_label_handle_t handle);
NU_API nu_result_t nusr_gui_label_set_position(nu_renderer_label_handle_t handle, int32_t x, int32_t y);
NU_API nu_result_t nusr_gui_label_set_text(nu_renderer_label_handle_t handle, const char *text);

#endif