#include "colorframebuffer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

using namespace nusr;

ColorFramebuffer::ColorFramebuffer(uint32_t width, uint32_t height)
    : Framebuffer<uint32_t>(width, height)
{

}

void ColorFramebuffer::display(const WindowInterface &interface)
{
    interface.presentSurface(m_width, m_height, m_data.data());
}
void ColorFramebuffer::save(const std::string &filename) const
{
    stbi_write_jpg(filename.c_str(), m_width, m_height, 4, m_data.data(), 100);
}
void ColorFramebuffer::blend(uint32_t x, uint32_t y, uint32_t value)
{
    uint32_t bvalue = get(x, y);

    uint32_t alpha = (value & 0xFF);
    uint32_t inv_alpha = 255 - alpha;

    bvalue >>= 8;
    value >>= 8;
    uint32_t rb = ((value & 0x00FF00FF) * alpha + (bvalue & 0x00FF00FF) * inv_alpha) & 0xFF00FF00;
    uint32_t g = ((value & 0x0000FF00) * alpha + (bvalue & 0x0000FF00) * inv_alpha) & 0x00FF0000;

    set(x, y, rb | g | 0xFF);
}