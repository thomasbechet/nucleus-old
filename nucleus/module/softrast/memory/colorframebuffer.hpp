#pragma once

#include "framebuffer.ipp"
#include "../engine/windowinterface.hpp"

#include <string>

namespace nu::softrast
{
    class ColorFramebuffer : public Framebuffer<uint32_t>
    {
    public:
        ColorFramebuffer(const Vector2u &size = Vector2u());
        
        void display(const WindowInterface &interface);
        void save(const std::string &filename) const;
        void blend(uint32_t x, uint32_t y, uint32_t value);
    };
}