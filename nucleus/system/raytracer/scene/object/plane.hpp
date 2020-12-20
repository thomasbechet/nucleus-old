#pragma once

#include "../object.hpp"

namespace nurt
{
    class Plane : public Object
    {
    public:
        Plane(
            const nu_mat4_t transform,
            MaterialHandle material
        );

        bool intersect(const nu_vec3_t pos, const nu_vec3_t dir, HitInfo &hitInfo) const override;
        void onTransformUpdated() override;

    private:
        nu_vec3_t m_normal;
        nu_vec3_t m_position;
    };
}