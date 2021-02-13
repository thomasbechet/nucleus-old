#include <nucleus/module/softrast/gui/gui.hpp>

#include <stdexcept>

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
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_labels.erase(id);
}
void GUI::setLabelPosition(nu_renderer_label_handle_t handle, const Vector2i &position)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_labels.at(id).position = position;
}
void GUI::setLabelText(nu_renderer_label_handle_t handle, const std::string &text)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_labels.at(id).text = text;
}

nu_renderer_rectangle_handle_t GUI::createRectangle(const nu_renderer_rectangle_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_rectangles.emplace(id, Rectangle(info));
    nu_renderer_rectangle_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void GUI::destroyRectangle(nu_renderer_rectangle_handle_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_rectangles.erase(id);
}
void GUI::setRectangleRect(nu_renderer_rectangle_handle_t handle, const Rect &rect)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_rectangles.at(id).rect = rect;
}

void GUI::renderLabel(ColorFramebuffer &colorBuffer, const Font &font, const Vector2i &position, const std::string &text) const
{
    int32_t currentX = position.x;

    // Iterate over characers
    for (char c : text) {
        try {

            const Glyph &g = font.glyphs.at(c);

            // Compute visibility bound
            Rect windowBound(0, 0, colorBuffer.getSize().x, colorBuffer.getSize().y);
            Rect characterBound(0, 0, g.bitmapSize.x, g.bitmapSize.y);

            // Translate character bound
            characterBound.left += (currentX + g.bearing.x);
            characterBound.top  += (position.y - g.bearing.y + font.lineHeight);

            // Clip and translate back character
            characterBound.clip(windowBound);
            characterBound.left -= (currentX + g.bearing.x);
            characterBound.top  -= (position.y - g.bearing.y + font.lineHeight);

            // Draw character
            const uint32_t startX = characterBound.left;
            const uint32_t stopX  = startX + characterBound.width;
            const uint32_t startY = characterBound.top;
            const uint32_t stopY  = startY + characterBound.height;
            for (uint32_t y = startY; y < stopY; y++) {
                for (uint32_t x = startX; x < stopX; x++) {
                    const uint32_t bufX = x + (currentX + g.bearing.x);
                    const uint32_t bufY = y + (position.y - g.bearing.y + font.lineHeight);

                    uint32_t color = font.atlas.get(x, g.ty + y);
                    colorBuffer.blend(bufX, bufY, color);
                }
            }

            // Goto next character
            currentX += g.advance.x;

        } catch(const std::out_of_range &e) {
            continue;
        }
    }
}
void GUI::renderRectangle(ColorFramebuffer &colorBuffer, const Rectangle &rectangle) const
{
    // Compute visibility bound
    Rect windowBound = {0, 0, colorBuffer.getSize().x, colorBuffer.getSize().y};
    Rect rect = rectangle.rect;
    rect.clip(windowBound);

    // Draw
    for (uint32_t y = rect.top; y < rect.top + rect.height; y++) {
        for (uint32_t x = rect.left; x < rect.left + rect.width; x++) {
            colorBuffer.blend(x, y, rectangle.color);
        }
    }
}

void GUI::draw(ColorFramebuffer &colorBuffer, const AssetManager &assetManager) const
{
    for (const auto &[key, value] : m_labels) {
        const Font &font = assetManager.getFont(value.font);
        renderLabel(colorBuffer, font, value.position, value.text);
    }
    for (const auto &[key, value] : m_rectangles) {
        renderRectangle(colorBuffer, value);
    }
}