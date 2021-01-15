#pragma once

#include <nucleus/nucleus.h>

#include <vector>

namespace nurt
{
    template<typename T>
    class Framebuffer
    {
    public:
        Framebuffer(uint32_t width, uint32_t height);
        
        void clear(const T &element);
        void set(uint32_t x, uint32_t y, const T &element);

    protected:
        uint32_t m_width;
        uint32_t m_height;
        std::vector<T> m_data;
    };
}