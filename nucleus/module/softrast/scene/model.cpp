#include "model.hpp"

using namespace nu::softrast;

Model::Model(const nu_renderer_model_create_info_t &info)
{
    mesh = info.mesh;
    materials.reserve(info.material_count);
    for (uint32_t i = 0; i < info.material_count; i++) {
        materials.emplace_back(info.materials[i]);
    }
    transform = nu::Matrix4f(info.transform);
}