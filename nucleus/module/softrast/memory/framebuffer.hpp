#pragma once

#include <nucleus/nucleus.h>

#include <vector>

namespace nusr
{
    template<typename T>
    class Framebuffer
    {
    public:
        Framebuffer(uint32_t width = 0, uint32_t height = 0);
        
        void clear(const T &element);
        void set(uint32_t x, uint32_t y, const T &element);
        T get(uint32_t x, uint32_t y) const;
        uint32_t getWidth() const;
        uint32_t getHeight() const;

    protected:
        uint32_t m_width;
        uint32_t m_height;
        std::vector<T> m_data;
    };
}