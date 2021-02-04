#include "gui.hpp"

using namespace nu::softrast;

nu_renderer_label_handle_t GUI::createLabel(const nu_renderer_label_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_labels.emplace(id, Label(info));
    nu_renderer_label_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void GUI::destroyLabel(nu_renderer_label_handle_t handle)
{
    m_labels.erase(NU_HANDLE_GET_ID(handle));
}
void GUI::setLabelPosition(nu_renderer_label_handle_t handle, const Vector2i &position)
{
    m_labels.at(NU_HANDLE_GET_ID(handle)).position = position;
}
void GUI::setLabelText(nu_renderer_label_handle_t handle, const std::string &text)
{
    m_labels.at(NU_HANDLE_GET_ID(handle)).text = text;
}

void GUI::renderLabel(ColorFramebuffer &colorBuffer, const Font &font, const Vector2i &position, const std::string &text) const
{
    int32_t currentX = position.x;

    // Iterate over characers
    for (char c : text) {
        const Glyph &g = font.glyphs.at(c);

        // Compute visibility bound
        Rect windowBound(0, 0, colorBuffer.getSize().x, colorBuffer.getSize().y);
        Rect characterBound(0, 0, g.bitmapSize.x, g.bitmapSize.y);

        // Translate character bound
        characterBound.left += (currentX + g.bearing.x);
        characterBound.top  += (position.y - g.bearing.y);

        // Clip and translate back character
        characterBound.clip(windowBound);
        characterBound.left -= (currentX + g.bearing.x);
        characterBound.top  -= (position.y - g.bearing.y);

        // Draw character
        const uint32_t startX = characterBound.left;
        const uint32_t stopX  = startX + characterBound.width;
        const uint32_t startY = characterBound.top;
        const uint32_t stopY  = startY + characterBound.height;
        for (uint32_t y = startY; y < stopY; y++) {
            for (uint32_t x = startX; x < stopX; x++) {
                const uint32_t bufX = x + (currentX + g.bearing.x);
                const uint32_t bufY = y + position.y - g.bearing.y;

                //uint32_t color = font.atlas[(g->ty + y) * font->width + x];
                //nusr_framebuffer_blend_uint(color_buffer, x_buf, y_buf, color);
                uint32_t color = font.atlas.get(x, g.ty + y);
                colorBuffer.blend(bufX, bufY, color);
            }
        }

        // Goto next character
        currentX += g.advance.x;
    }
}

void GUI::draw(ColorFramebuffer &colorBuffer, const AssetManager &assetManager) const
{
    for (const auto &[key, value] : m_labels) {
        const Font &font = assetManager.getFont(value.font);
        renderLabel(colorBuffer, font, value.position, value.text);
    }
}