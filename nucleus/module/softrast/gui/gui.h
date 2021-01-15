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

typedef struct {
    nu_rect_t rect;
    uint32_t color;
    bool active;
} nusr_rectangle_t;

nu_result_t nusr_gui_initialize(void);
nu_result_t nusr_gui_terminate(void);
nu_result_t nusr_gui_render(nusr_framebuffer_t *color_buffer);

nu_result_t nusr_gui_label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info);
nu_result_t nusr_gui_label_destroy(nu_renderer_label_handle_t handle);
nu_result_t nusr_gui_label_set_position(nu_renderer_label_handle_t handle, int32_t x, int32_t y);
nu_result_t nusr_gui_label_set_text(nu_renderer_label_handle_t handle, const char *text);

nu_result_t nusr_gui_rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info);
nu_result_t nusr_gui_rectangle_destroy(nu_renderer_rectangle_handle_t handle);
nu_result_t nusr_gui_rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect);

#endif