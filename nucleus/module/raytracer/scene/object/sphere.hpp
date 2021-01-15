#pragma once

#include "../object.hpp"

namespace nurt
{
    class Sphere : public Object
    {
    public:
        Sphere(
            const nu_mat4f_t transform,
            MaterialHandle material,
            float radius
        );

        bool intersect(const nu_vec3f_t pos, const nu_vec3f_t dir, HitInfo &hitInfo) const override;
        void onTransformUpdated() override;

    private:
        float m_radius;
        nu_vec3f_t m_center;
    };
}