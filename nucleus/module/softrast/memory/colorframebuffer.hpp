#pragma once

#include <nucleus/module/softrast/memory/framebuffer.ipp>
#include <nucleus/module/softrast/engine/windowinterface.hpp>

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