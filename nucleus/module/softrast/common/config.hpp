#pragma once

#include <string>

namespace nu::softrast
{
    class Config
    {
    public:
        static inline const std::string Section           = "softrast";
        static inline const std::string FramebufferWidth  = "framebuffer_width";
        static inline const std::string FramebufferHeight = "framebuffer_height";
    };
}