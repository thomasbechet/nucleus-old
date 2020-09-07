#ifndef NUSR_VIEWPORT_H
#define NUSR_VIEWPORT_H

#include "../memory/framebuffer.h"

typedef struct {
    nusr_framebuffer_t color_buffer;
    nusr_framebuffer_t depth_buffer;
} nusr_renderbuffer_t;

nu_result_t nusr_viewport_initialize(void);
nu_result_t nusr_viewport_terminate(void);
nu_result_t nusr_viewport_get_renderbuffer(nusr_renderbuffer_t *renderbuffer);

#endif