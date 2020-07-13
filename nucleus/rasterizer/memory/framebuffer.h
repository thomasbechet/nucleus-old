#ifndef NURZ_FRAMEBUFFER_H
#define NURZ_FRAMEBUFFER_H

#include "../../core/common/common.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t *pixels;
} nurz_framebuffer_t;

nu_result_t nurz_framebuffer_create(nurz_framebuffer_t *self, uint32_t width, uint32_t height);
nu_result_t nurz_framebuffer_destroy(nurz_framebuffer_t *self);
nu_result_t nurz_framebuffer_clear(nurz_framebuffer_t *self, uint32_t color);
nu_result_t nurz_framebuffer_set_rgb(nurz_framebuffer_t *self,
    uint32_t x, uint32_t y,
    float r, float g, float b
);

#endif