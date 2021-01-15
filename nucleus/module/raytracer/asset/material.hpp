#pragma once

#include <nucleus/nucleus.h>

namespace nurt
{
    NU_DECLARE_HANDLE(MaterialHandle);

    struct Material
    {
        MaterialHandle handle;
        nu_vec3f_t albedo;
    };
}