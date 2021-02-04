#pragma once

#include <nucleus/nucleus.hpp>

#include <vector>

namespace nu::softrast
{
    template<typename T>
    class Framebuffer
    {
    public:
        Framebuffer(const Vector2u &size = Vector2u());
        
        void clear(const T &element);
        void set(uint32_t x, uint32_t y, const T &element);
        T get(uint32_t x, uint32_t y) const;
        Vector2u getSize() const;

    protected:
        Vector2u m_size;
        std::vector<T> m_data;
    };
}