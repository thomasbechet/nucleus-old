#pragma once

#include "framebuffer.hpp"

namespace nurt
{
    template<typename T>
    Framebuffer<T>::Framebuffer(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;
        m_data.resize(width * height);
    }
    
    template<typename T>
    void Framebuffer<T>::clear(const T &element)
    {
        std::fill(m_data.begin(), m_data.end(), element);
    }
    template<typename T>
    void Framebuffer<T>::set(uint32_t x, uint32_t y, const T &element)
    {
        m_data[y * m_width + x] = element;
    }
}