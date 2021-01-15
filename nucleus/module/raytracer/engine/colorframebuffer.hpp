#pragma once

#include "framebuffer.ipp"
#include "windowinterface.hpp"

#include <string>

namespace nurt
{
    struct Color4
    {
        nu_vec4f_t data;
    };

    class ColorFramebuffer : public Framebuffer<Color4>
    {
    public:
        ColorFramebuffer(uint32_t width, uint32_t height);
        
        void display(const WindowInterface &interface);
        void save(const std::string &filename) const;
        void setColor(uint32_t width, uint32_t height, const nu_vec4f_t color);
        
        // Override methods
        void clear(const nu_vec4f_t color);
    };
}