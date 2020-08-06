#ifndef NUSR_FRAMEBUFFER_H
#define NUSR_FRAMEBUFFER_H

#include "../../core/common/common.h"

typedef union {
    uint32_t as_uint;
    float as_float;
} nusr_framebuffer_pixel_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    nusr_framebuffer_pixel_t *pixels;
} nusr_framebuffer_t;

nu_result_t nusr_framebuffer_create(nusr_framebuffer_t *self, uint32_t width, uint32_t height);
nu_result_t nusr_framebuffer_destroy(nusr_framebuffer_t *self);
nu_result_t nusr_framebuffer_clear(nusr_framebuffer_t *self, uint32_t color);
nu_result_t nusr_framebuffer_set_rgb(nusr_framebuffer_t *self,
    uint32_t x, uint32_t y,
    float r, float g, float b
);

#endif