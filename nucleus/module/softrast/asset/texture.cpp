#include "texture.hpp"

using namespace nusr;

Texture::Texture(const nu_renderer_texture_create_info_t &info)
 : ColorFramebuffer(info.height, info.width)
{
    for (uint32_t p = 0; p < info.width * info.height; p++) {
        uint32_t color = ((uint32_t)(info.data[p * 3 + 0]) << 24) + ((uint32_t)(info.data[p * 3 + 1]) << 16) + ((uint32_t)(info.data[p * 3 + 2]) << 8);
        m_data[p] = color;
    }
}