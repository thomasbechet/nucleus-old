#include "line.hpp"

#include "logger.hpp"

using namespace nu::utility;
using namespace nu;

Line::Line(nu_renderer_font_handle_t font)
{
    // Create label
    nu_renderer_label_create_info_t label_info;
    nu_vec2i_copy(m_position, label_info.position);
    label_info.font = font;
    label_info.text = "";
    if (nu_renderer_label_create(&m_handle, &label_info) != NU_SUCCESS) {
        nu_interrupt(NUUTILS_LOGGER_NAME"Failed to create command label.\n");
    }
}
Line::~Line()
{
    // Destroy label
    nu_renderer_label_destroy(m_handle);
}

void Line::setPosition(const Vector2i &position)
{
    m_position = position;
    nu_renderer_label_set_position(m_handle, m_position);
}
void Line::setText(const std::string &text)
{
    m_text = text;
    updateLabelText();
}
void Line::setVisible(bool toggle)
{
    m_visible = toggle;
    updateLabelText();
}
std::string Line::getText() const
{
    return m_text;
}

void Line::updateLabelText()
{
    if (m_visible) {
        nu_renderer_label_set_text(m_handle, m_text.c_str());
    } else {
        nu_renderer_label_set_text(m_handle, "");
    }
}