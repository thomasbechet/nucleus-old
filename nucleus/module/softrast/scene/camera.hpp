#pragma once

#include <nucleus/nucleus.hpp>

namespace nu::softrast
{
    struct Camera
    {
        Matrix4f view;
        float fov;
        float near;
        float far;
    };
}