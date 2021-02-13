#include <nucleus/core/common/math/rect.h>

#include <nucleus/core/common/macro.h>

void nu_rect_clip(nu_rect_t *rect, const nu_rect_t *bound)
{
    int32_t left = NU_MIN(bound->left + (int32_t)bound->width, NU_MAX(rect->left, bound->left));
    int32_t right = NU_MIN(bound->left + (int32_t)bound->width, NU_MAX(rect->left + (int32_t)rect->width, bound->left));
    int32_t top = NU_MIN(bound->top + (int32_t)bound->height, NU_MAX(rect->top, bound->top));
    int32_t bottom = NU_MIN(bound->top + (int32_t)bound->height, NU_MAX(rect->top + (int32_t)rect->height, bound->top));

    rect->left = left;
    rect->top = top;
    rect->width = right - left;
    rect->height = bottom - top;
}