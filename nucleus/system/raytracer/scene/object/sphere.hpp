#pragma once

#include "../object.hpp"

namespace nurt
{
    class Sphere : public Object
    {
    public:
        Sphere(
            const nu_mat4_t transform,
            MaterialHandle material,
            float radius
        );

        bool intersect(const nu_vec3_t pos, const nu_vec3_t dir, HitInfo &hitInfo) const override;
        void onTransformUpdated() override;

    private:
        float m_radius;
        nu_vec3_t m_center;
    };
}