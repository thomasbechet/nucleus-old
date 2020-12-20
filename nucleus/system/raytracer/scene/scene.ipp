#pragma once

#include "scene.hpp"

namespace nurt
{
    template<typename T, typename... Args>
    void Scene::add(Args... args)
    {
        m_objects.emplace_back(std::make_unique<T>(args...));
    }
}