#pragma once

#include <nucleus/nucleus.h>

namespace nu
{
    class Rect
    {
    public:
        union{struct{int32_t left; int32_t top; uint32_t width; uint32_t height;}; nu_rect_t data;};
        Rect(){left = top = width = height = 0;}
        Rect(const nu_rect_t rect){data = rect;}
        Rect(int32_t l, int32_t t, uint32_t w, uint32_t h){left = l; top = t; width = w; height = h;};
        operator const nu_rect_t*()const{return &data;}

    public:
        void clip(const Rect &rect){nu_rect_clip(&data, rect);}
    };
}