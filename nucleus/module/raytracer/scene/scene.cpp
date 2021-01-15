#include "scene.hpp"

using namespace nurt;

bool Scene::trace(
    const nu_vec3f_t pos,
    const nu_vec3f_t dir,
    HitInfo &hitInfo    
) const
{
    bool hit = false;
    hitInfo.distance = std::numeric_limits<float>::infinity();
    for (auto& object : m_objects) {
        HitInfo tempHitInfo;
        if (object->intersect(pos, dir, tempHitInfo)) {
            hit = true;
            if (tempHitInfo.distance < hitInfo.distance) {
                hitInfo = tempHitInfo;
            }
        }
    }

    return hit;
}