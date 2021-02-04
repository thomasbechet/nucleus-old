#include "material.hpp"

using namespace nu::softrast;

Material::Material(const nu_renderer_material_create_info_t &info)
{
    texture = info.diffuse_texture;
}