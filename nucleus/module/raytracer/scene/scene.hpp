#pragma once

#include "object.hpp"

#include <nucleus/nucleus.h>

#include <vector>
#include <memory>

namespace nurt
{
    class Scene
    {
    public:
        bool trace(
            const nu_vec3f_t pos,
            const nu_vec3f_t dir,
            HitInfo &hitInfo
        ) const;

        template<typename T, typename... Args>
        void add(Args...);

    private:
        std::vector<std::unique_ptr<Object>> m_objects;
    };
}