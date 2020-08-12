#include "font.h"

#include "../common/logger.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define MAX_FONT_COUNT 8
#define MIN_CHAR_CODE 32
#define MAX_CHAR_CODE 128

typedef struct {
    nusr_font_t **fonts;
    uint32_t next_id;
    FT_Library freetype;
} nusr_asset_font_data_t;

static nusr_asset_font_data_t _data;

nu_result_t nusr_font_initialize(void)
{
    /* allocate memory */
    _data.next_id = 0;
    _data.fonts = (nusr_font_t**)nu_malloc(sizeof(nusr_font_t*) * MAX_FONT_COUNT);
    memset(_data.fonts, 0, sizeof(nusr_font_t*) * MAX_FONT_COUNT);

    /* initialize FreeType */
    FT_Error error = FT_Init_FreeType(&_data.freetype);
    if (error) {
        nu_warning(NUSR_LOGGER_NAME"Failed to initialize FreeType.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nusr_font_terminate(void)
{
    /* terminate FreeType */
    FT_Done_FreeType(_data.freetype);

    /* free memory */
    for (uint32_t i = 0; i < _data.next_id; i++) {
        if (_data.fonts[i]) {
            nusr_font_destroy(i);
        }
    }

    nu_free(_data.fonts);

    return NU_SUCCESS;
}

nu_result_t nusr_font_create(uint32_t *id, const char* filename)
{
    FT_Face face;
    FT_GlyphSlot glyph;
    FT_Error error;

    /* error check */
    if (_data.next_id >= MAX_FONT_COUNT) return NU_FAILURE;

    /* load face */ 
    error = FT_New_Face(_data.freetype, filename, 0, &face);
    if (error) {
        if (error == FT_Err_Unknown_File_Format) {
            nu_warning(NUSR_LOGGER_NAME"Unknown file format.\n");
        }
        nu_warning(NUSR_LOGGER_NAME"Failed to load font.\n");
    
        return NU_FAILURE;
    }
    glyph = face->glyph;

    /* set pixel sizes */
    error = FT_Set_Pixel_Sizes(face, 0, 16);
    if (error) {
        nu_warning(NUSR_LOGGER_NAME"Failed to set font pixel sizes.\n");
    }

    /* create font */
    _data.fonts[_data.next_id] = (nusr_font_t*)nu_malloc(sizeof(nusr_font_t));

    /* find altas texture sizes */
    uint32_t w = 0;
    uint32_t h = 0;
    for (uint32_t c = MIN_CHAR_CODE; c < MAX_CHAR_CODE; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            nu_warning(NUSR_LOGGER_NAME"Failed to load character %c.\n", c);
            continue;
        }

        w = NU_MAX(glyph->bitmap.width, h);
        h += glyph->bitmap.rows;
    }

    /* allocate memory */
    _data.fonts[_data.next_id]->glyph_count = (MAX_CHAR_CODE - MIN_CHAR_CODE);
    _data.fonts[_data.next_id]->glyphs = (nusr_glyph_t*)nu_malloc(sizeof(nusr_glyph_t) * _data.fonts[_data.next_id]->glyph_count);

    _data.fonts[_data.next_id]->width = w;
    _data.fonts[_data.next_id]->height = h;
    _data.fonts[_data.next_id]->atlas = (uint32_t*)nu_malloc(sizeof(uint32_t) * w * h);

    /* load glyphs */
    h = 0;
    for (uint32_t c = MIN_CHAR_CODE; c < MAX_CHAR_CODE; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        for (uint32_t y = 0; y < glyph->bitmap.rows; y++) {
            for (uint32_t x = 0; x < glyph->bitmap.width; x++) {
                unsigned char color = glyph->bitmap.buffer[y * glyph->bitmap.rows + x];
                _data.fonts[_data.next_id]->atlas[y * w + x] = color > 0 ? 0xFFFFFFFF : 0x0; 
            }
        }
        h += glyph->bitmap.rows;
    
        nusr_glyph_t *g = &_data.fonts[_data.next_id]->glyphs[c - MIN_CHAR_CODE];
        g->ax = glyph->advance.x >> 6;
        g->ay = glyph->advance.y >> 6;
        g->bw = glyph->bitmap.width;
        g->bh = glyph->bitmap.rows;
    }

    /* save id */
    *id = _data.next_id++;

    return NU_SUCCESS;
}
nu_result_t nusr_font_destroy(uint32_t id)
{
    if (_data.next_id >= MAX_FONT_COUNT) return NU_FAILURE;
    if (!_data.fonts[id]) return NU_FAILURE;

    nu_free(_data.fonts[id]);
    _data.fonts[id] = NULL;

    return NU_SUCCESS;
}
nu_result_t nusr_font_get(uint32_t id, nusr_font_t **p)
{
    if (_data.next_id >= MAX_FONT_COUNT) return NU_FAILURE;
    if (!_data.fonts[id]) return NU_FAILURE;

    *p = _data.fonts[id];

    return NU_SUCCESS;
}