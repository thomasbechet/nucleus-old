#ifndef NUSR_FONT_H
#define NUSR_FONT_H

#include "../module/interface.h"

typedef struct {
    uint32_t advance_x;
    uint32_t advance_y;

    uint32_t bitmap_width;
    uint32_t bitmap_height;

    uint32_t bearing_x;
    uint32_t bearing_y;

    uint32_t ty;
} nusr_glyph_t;

typedef struct {
    uint32_t *atlas;
    uint32_t height;
    uint32_t width;
    nusr_glyph_t *glyphs;
    uint32_t glyph_count;
} nusr_font_t;

nu_result_t nusr_font_initialize(void);
nu_result_t nusr_font_terminate(void);

nu_result_t nusr_font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info);
nu_result_t nusr_font_destroy(nu_renderer_font_handle_t handle);
nu_result_t nusr_font_get(uint32_t id, nusr_font_t **p);

#endif