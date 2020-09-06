#include "rect.h"

#include "../macro.h"

void nu_rect_clip(nu_rect_t *rect, const nu_rect_t *bound)
{
    if ((rect->left + rect->width) > (bound->left + bound->width)) {
        rect->width = 
    }
    rect->left = NU_MAX(rect->left, bound->left);
    rect->top = NU_MAX(rect->top, bound->top);
    if () {

    }
}