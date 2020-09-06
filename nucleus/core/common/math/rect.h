#ifndef NU_RECT_H
#define NU_RECT_H

#include <stdint.h>

typedef struct {
    int32_t top;
    int32_t left;
    uint32_t width;
    uint32_t height;
} nu_rect_t;

NU_API void nu_rect_clip(nu_rect_t *rect, const nu_rect_t *bound);

#endif