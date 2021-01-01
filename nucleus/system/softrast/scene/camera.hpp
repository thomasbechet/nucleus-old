#pragma once

#include <nucleus/nucleus.h>

namespace nusr
{
    class Camera
    {
    public:
        nu_vec3_t eye;
        nu_vec3_t center;
        nu_vec3_t up;
        float fov;
        float near;
        float far;
    };
}