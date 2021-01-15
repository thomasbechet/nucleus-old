#pragma once

#include "../asset/material.hpp"

#include <nucleus/nucleus.h>

namespace nurt
{
    struct HitInfo
    {
        nu_vec3f_t position;
        nu_vec3f_t normal;
        float distance;
        MaterialHandle material;
    };
}