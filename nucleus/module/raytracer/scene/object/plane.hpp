#pragma once

#include "../object.hpp"

namespace nurt
{
    class Plane : public Object
    {
    public:
        Plane(
            const nu_mat4f_t transform,
            MaterialHandle material
        );

        bool intersect(const nu_vec3f_t pos, const nu_vec3f_t dir, HitInfo &hitInfo) const override;
        void onTransformUpdated() override;

    private:
        nu_vec3f_t m_normal;
        nu_vec3f_t m_position;
    };
}