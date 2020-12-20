#pragma once

#include "../asset/material.hpp"
#include "hitinfo.hpp"

namespace nurt
{
    class Object
    {
    public:
        Object(
            const nu_mat4_t transform,
            MaterialHandle material
        );
        ~Object() = default;

        void setTransform(const nu_mat4_t transform);

        virtual bool intersect(const nu_vec3_t pos, const nu_vec3_t dir, HitInfo &hitInfo) const = 0;
        virtual void onTransformUpdated() = 0;

    protected:
        nu_mat4_t m_transform;
        MaterialHandle m_material;
    };
}