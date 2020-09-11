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

static nu_result_t create_font(uint32_t *id, const nu_renderer_font_create_info_t *info)
{
    FT_Face face;
    FT_GlyphSlot glyph;
    FT_Error error;

    /* error check */
    if (_data.next_id >= MAX_FONT_COUNT) return NU_FAILURE;

    /* load face */ 
    error = FT_New_Face(_data.freetype, info->filename, 0, &face);
    if (error) {
        if (error == FT_Err_Unknown_File_Format) {
            nu_warning(NUSR_LOGGER_NAME"Unknown file format.\n");
        }
        nu_warning(NUSR_LOGGER_NAME"Failed to load font.\n");
    
        return NU_FAILURE;
    }
    glyph = face->glyph;

    /* set pixel sizes */
    error = FT_Set_Pixel_Sizes(face, 0, info->font_size);
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

        w = NU_MAX(glyph->bitmap.width, w);
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

        if (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) continue;

        nusr_glyph_t *g = &_data.fonts[_data.next_id]->glyphs[c - MIN_CHAR_CODE];
        g->advance_x = glyph->advance.x >> 6;
        g->advance_y = glyph->advance.y >> 6;
        g->bitmap_width = glyph->bitmap.width;
        g->bitmap_height = glyph->bitmap.rows;
        g->bearing_x = (glyph->metrics.horiBearingX >> 6);
        g->bearing_y = (glyph->metrics.horiBearingY >> 6);
        g->ty = h;

        /* fill texture atlas */
        for (uint32_t y = 0; y < g->bitmap_height; y++) {
            for (uint32_t x = 0; x < g->bitmap_width; x++) {
                unsigned char byte = (uint32_t)glyph->bitmap.buffer[y * glyph->bitmap.width + x];
                _data.fonts[_data.next_id]->atlas[(h + y) * w + x] = (0xFFFFFF00 | byte);
            }
        }

        h += g->bitmap_height;
    }

    /* save id */
    *id = _data.next_id++;

    return NU_SUCCESS;
}
static nu_result_t destroy_font(uint32_t id)
{
    if (_data.next_id >= MAX_FONT_COUNT) return NU_FAILURE;
    if (!_data.fonts[id]) return NU_FAILURE;

    nu_free(_data.fonts[id]->atlas);
    nu_free(_data.fonts[id]->glyphs);

    nu_free(_data.fonts[id]);
    _data.fonts[id] = NULL;

    return NU_SUCCESS;
}

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
            destroy_font(i);
        }
    }

    nu_free(_data.fonts);

    return NU_SUCCESS;
}
nu_result_t nusr_font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    uint32_t id;
    nu_result_t result = create_font(&id, info);
    if (result == NU_SUCCESS) *((uint32_t*)handle) = id;
    return result;
}
nu_result_t nusr_font_destroy(nu_renderer_font_handle_t handle)
{
    uint32_t id = (uint64_t)handle;
    return destroy_font(id);
}
nu_result_t nusr_font_get(uint32_t id, nusr_font_t **p)
{
    if (_data.next_id >= MAX_FONT_COUNT) return NU_FAILURE;
    if (!_data.fonts[id]) return NU_FAILURE;

    *p = _data.fonts[id];

    return NU_SUCCESS;
}