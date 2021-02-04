#pragma once

#include "framebuffer.hpp"

namespace nu::softrast
{
    template<typename T>
    Framebuffer<T>::Framebuffer(const Vector2u &size)
    {
        m_size = size;
        m_data.resize(m_size.x * m_size.y);
    }
    
    template<typename T>
    void Framebuffer<T>::clear(const T &element)
    {
        std::fill(m_data.begin(), m_data.end(), element);
    }
    template<typename T>
    void Framebuffer<T>::set(uint32_t x, uint32_t y, const T &element)
    {
        m_data[y * m_size.x + x] = element;
    }
    template<typename T>
    T Framebuffer<T>::get(uint32_t x, uint32_t y) const
    {
        return m_data[y * m_size.x + x];
    }
    template<typename T>
    Vector2u Framebuffer<T>::getSize() const
    {
        return m_size;
    }
}