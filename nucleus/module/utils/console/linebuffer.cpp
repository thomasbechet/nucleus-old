#include <nucleus/module/utils/console/linebuffer.hpp>

using namespace nu::utility;
using namespace nu;

LineBuffer::LineBuffer(nu_renderer_font_handle_t font, uint32_t maxLine)
{
    m_font = font;
    Vector2u size;
    nu_renderer_font_get_text_size(m_font, "A", size.data);
    m_labelHeight = size.y;
    m_maxLine = maxLine;
}

void LineBuffer::add(const std::string &text)
{
    if (m_lines.size() >= m_maxLine) {
        m_lines.erase(m_lines.begin());
    }

    m_lines.emplace_back(std::make_unique<Line>(m_font));
    m_lines.back()->setText(text);
    m_lines.back()->setVisible(m_visible);
    updateLinesPosition();
}
void LineBuffer::clear()
{
    m_lines.clear();
}
void LineBuffer::setPosition(const Vector2i &position)
{
    m_position = position;
    updateLinesPosition();
}
void LineBuffer::setVisible(bool visible)
{
    for (auto &line : m_lines) {
        line->setVisible(visible);
    }
}

void LineBuffer::updateLinesPosition()
{
    Vector2i currentPosition = m_position;
    for (auto line = m_lines.rbegin(); line != m_lines.rend(); ++line) { 
        line->get()->setPosition(currentPosition);
        currentPosition.y -= (m_labelHeight + 1);
    }
}