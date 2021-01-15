#pragma once

#include <nucleus/nucleus.h>

namespace nurt
{
    class Light
    {
    public:
        Light(const nu_vec3f_t color, float intensity);
        ~Light() = default;

        virtual void illumate(const nu_vec3f_t position, const nu_vec3f_t normal, nu_vec3f_t color) const = 0;

    protected:
        nu_vec3f_t m_color;
        float m_intensity;
    };
}