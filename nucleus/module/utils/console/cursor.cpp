#include <nucleus/module/utils/console/cursor.hpp>

using namespace nu::utility;
using namespace nu;

Cursor::Cursor(float frequency)
{
    m_rect.top = 0;
    m_rect.left = 0;
    m_rect.width = 5;
    m_rect.height = 1;

    m_delta = 0.0f;
    m_frequency = frequency;

    m_visible = true;

    nu_renderer_rectangle_create_info_t rectangleInfo;
    rectangleInfo.color = 0xFFFFFFFF;
    rectangleInfo.rect = m_rect.data;
    nu_renderer_rectangle_create(&rectangleInfo, &m_handle);

    updateRectangle();
}
Cursor::~Cursor()
{
    nu_renderer_rectangle_destroy(m_handle);
}

void Cursor::setPosition(const Vector2i &position)
{
    m_rect.left = position.x;
    m_rect.top  = position.y;
    updateRectangle();
}
void Cursor::setVisible(bool visible)
{
    m_visible = visible;
    updateRectangle();
}
void Cursor::setAdvance(uint32_t advance)
{
    m_advance = advance;
    m_rect.height = 1;
    updateRectangle();
}

void Cursor::update(float delta)
{
    if (m_visible) {
        m_delta += delta;
        if (m_delta >= m_frequency) {
            m_delta = 0;
            m_rect.height = m_rect.height > 0 ? 0 : 1;
            updateRectangle();
        }
    }
}

void Cursor::updateRectangle()
{
    Rect rect = m_rect;
    rect.left += m_advance;
    if (!m_visible) rect.height = 0;
    nu_renderer_rectangle_set_rect(m_handle, rect.data);
}