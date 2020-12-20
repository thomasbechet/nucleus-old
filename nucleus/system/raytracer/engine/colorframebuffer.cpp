#include "colorframebuffer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

using namespace nurt;

ColorFramebuffer::ColorFramebuffer(uint32_t width, uint32_t height)
    : Framebuffer<Color4>(width, height)
{

}

void ColorFramebuffer::display(const WindowInterface &interface)
{
    std::vector<uint32_t> data;
    data.resize(m_data.size());
    for (uint32_t i = 0; i < m_data.size(); i++) {
        data[i] = 
            ((uint32_t)(m_data[i].data[0] * 255.0f) & 0xFF) << 24 |
            ((uint32_t)(m_data[i].data[1] * 255.0f) & 0xFF) << 16 |
            ((uint32_t)(m_data[i].data[2] * 255.0f) & 0xFF) << 8  |
            ((uint32_t)(m_data[i].data[3] * 255.0f) & 0xFF) << 0;
    }

    interface.presentSurface(m_width, m_height, data.data());
}
void ColorFramebuffer::save(const std::string &filename) const
{
    std::vector<uint32_t> data;
    data.resize(m_data.size());
    for (uint32_t i = 0; i < m_data.size(); i++) {
        data[i] = 
            ((uint32_t)(m_data[i].data[0] * 255.0f) & 0xFF) << 0  |
            ((uint32_t)(m_data[i].data[1] * 255.0f) & 0xFF) << 8  |
            ((uint32_t)(m_data[i].data[2] * 255.0f) & 0xFF) << 16 |
            ((uint32_t)(m_data[i].data[3] * 255.0f) & 0xFF) << 24;
    }

    stbi_write_jpg(filename.c_str(), m_width, m_height, 4, data.data(), 100);
}
void ColorFramebuffer::setColor(uint32_t x, uint32_t y, const nu_vec4_t color)
{
    nu_vec4_copy(color, m_data[y * m_width + x].data);
}

void ColorFramebuffer::clear(const nu_vec4_t color)
{
    Color4 clearColor;
    nu_vec4_copy(color, clearColor.data);
    Framebuffer<Color4>::clear(clearColor);
}