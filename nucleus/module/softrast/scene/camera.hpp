#pragma once

#include <nucleus/nucleus.h>

namespace nusr
{
    class Camera
    {
    public:
        nu_mat4f_t view;
        float fov;
        float near;
        float far;
    };
}