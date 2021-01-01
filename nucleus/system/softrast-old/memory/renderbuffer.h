#ifndef NUSR_RENDERBUFFER_H
#define NUSR_RENDERBUFFER_H

#include "framebuffer.h"

typedef struct {
    nusr_framebuffer_t color_buffer;
    nusr_framebuffer_t depth_buffer;
} nusr_renderbuffer_t;

nu_result_t nusr_renderbuffer_create(nusr_renderbuffer_t *self, uint32_t width, uint32_t height);
nu_result_t nusr_renderbuffer_destroy(nusr_renderbuffer_t *self);

#endif