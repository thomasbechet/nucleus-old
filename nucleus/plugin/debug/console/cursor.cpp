#include "cursor.hpp"

using namespace nudebug;

cursor_t::cursor_t(float frequency)
{
    m_rect.top = 0;
    m_rect.left = 0;
    m_rect.width = 5;
    m_rect.height = 1;

    m_delta = 0.0f;
    m_frequency = frequency;

    m_visible = true;

    nu_renderer_rectangle_create_info_t rectangle_info;
    rectangle_info.color = 0xFFFFFFFF;
    rectangle_info.rect = m_rect;
    nu_renderer_rectangle_create(&m_handle, &rectangle_info);

    update_rectangle();
}
cursor_t::~cursor_t()
{
    nu_renderer_rectangle_destroy(m_handle);
}

void cursor_t::set_position(uint32_t x, uint32_t y)
{
    m_rect.left = x;
    m_rect.top = y;
    update_rectangle();
}
void cursor_t::set_visible(bool visible)
{
    m_visible = visible;
    update_rectangle();
}

void cursor_t::update(float delta)
{
    if (m_visible) {
        m_delta += delta;
        if (m_delta >= m_frequency) {
            m_delta = 0;
            m_rect.height = m_rect.height > 0 ? 0 : 1;
            update_rectangle();
        }
    }
}

void cursor_t::update_rectangle()
{
    nu_rect_t rect = m_rect;
    rect.height = m_visible ? 1 : 0;
    nu_renderer_rectangle_set_rect(m_handle, rect);
}