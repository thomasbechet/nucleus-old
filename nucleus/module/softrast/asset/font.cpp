#include <nucleus/module/softrast/asset/font.hpp>

#include <stdexcept>
#include <ft2build.h>
#include FT_FREETYPE_H

#define MIN_CHAR_CODE 32
#define MAX_CHAR_CODE 128

using namespace nu;
using namespace nu::softrast;

static FT_Library initializeFTLibrary()
{
    /* initialize FreeType */
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        throw std::runtime_error("Failed to initialize FreeType.");
    }
    return library;
}

Font::Font(const nu_renderer_font_create_info_t &info)
{
    static FT_Library freetype = initializeFTLibrary();

    FT_GlyphSlot glyph;

    // Load face
    FT_Face face;
    FT_Error error = FT_New_Face(freetype, info.filename, 0, &face);
    if (error) {
        if (error == FT_Err_Unknown_File_Format) {
            throw std::runtime_error("Unknown file format.");
        }
        throw std::runtime_error("Failed to load font.");
    }
    glyph = face->glyph;

    // Set pixel sizes
    error = FT_Set_Pixel_Sizes(face, 0, info.font_size);
    if (error) {
        throw std::runtime_error("Failed to set font pixel sizes.");
    }

    // Find altas texture size
    Vector2u atlasSize = {0, 0};
    for (uint32_t c = MIN_CHAR_CODE; c < MAX_CHAR_CODE; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error("Failed to load character " + c);
        }

        atlasSize.x = NU_MAX(glyph->bitmap.width, atlasSize.x);
        atlasSize.y += glyph->bitmap.rows;
    }

    // Allocate memory for atlas
    atlas = ColorFramebuffer(atlasSize);

    // Load glyphs
    uint32_t h = 0;
    for (uint32_t c = MIN_CHAR_CODE; c < MAX_CHAR_CODE; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

        if (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) continue;

        Glyph g;
        g.advance.x    = glyph->advance.x >> 6;
        g.advance.y    = glyph->advance.y >> 6;
        g.bitmapSize.x = glyph->bitmap.width;
        g.bitmapSize.y = glyph->bitmap.rows;
        g.bearing.x    = (glyph->metrics.horiBearingX >> 6);
        g.bearing.y    = (glyph->metrics.horiBearingY >> 6);
        g.ty           = h;

        // Fill atlas
        for (uint32_t y = 0; y < g.bitmapSize.y; y++) {
            for (uint32_t x = 0; x < g.bitmapSize.x; x++) {
                unsigned char byte = (uint32_t)glyph->bitmap.buffer[y * glyph->bitmap.width + x];
                atlas.set(x, h + y, 0xFFFFFF00 | byte);
            }
        }

        h += g.bitmapSize.y;
    
        glyphs.emplace(c, g);
    }

    // Save optimal line height
    lineHeight = face->size->metrics.height >> 6;
}

Vector2u Font::getTextSize(const std::string &text) const
{
    Vector2u maxSize = {0, lineHeight};

    for (char c : text) {
        try {
            Glyph g = glyphs.at(c);
            /* compute max height and width */
            maxSize.x += g.advance.x;
        } catch(std::out_of_range &e) {
            continue;
        }
    }

    return maxSize;
}