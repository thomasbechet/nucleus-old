#pragma once

#include "framebuffer.ipp"
#include "../engine/windowinterface.hpp"

#include <string>

namespace nusr
{
    class ColorFramebuffer : public Framebuffer<uint32_t>
    {
    public:
        ColorFramebuffer(uint32_t width = 0, uint32_t height = 0);
        
        void display(const WindowInterface &interface);
        void save(const std::string &filename) const;
        void blend(uint32_t x, uint32_t y, uint32_t value);
    };
}