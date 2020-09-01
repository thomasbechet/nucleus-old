#include "framebuffer.h"

nu_result_t nusr_framebuffer_create(nusr_framebuffer_t *self, uint32_t width, uint32_t height)
{
    self->pixels = (nusr_framebuffer_pixel_t*)nu_malloc(sizeof(nusr_framebuffer_pixel_t) * width * height);
    self->width = width;
    self->height = height;

    return NU_SUCCESS;
}
nu_result_t nusr_framebuffer_destroy(nusr_framebuffer_t *self)
{
    nu_free(self->pixels);
    self->pixels = NULL;
    self->width = 0;
    self->height = 0;

    return NU_SUCCESS;
}
nu_result_t nusr_framebuffer_clear(nusr_framebuffer_t *self, uint32_t color)
{
    memset(self->pixels, color, sizeof(nusr_framebuffer_pixel_t) * self->width * self->height);

    return NU_SUCCESS;
}
nu_result_t nusr_framebuffer_set_rgb(nusr_framebuffer_t *self,
    uint32_t x, uint32_t y,
    float fr, float fg, float fb
)
{
    uint32_t r = (uint32_t)(fr * 255.0f);
    uint32_t g = (uint32_t)(fg * 255.0f);
    uint32_t b = (uint32_t)(fb * 255.0f);
    nusr_framebuffer_set_uint(self, x, y, ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8));

    return NU_SUCCESS;
}
nu_result_t nusr_framebuffer_set_uint(nusr_framebuffer_t *self, uint32_t x, uint32_t y, uint32_t value)
{
    self->pixels[(self->height - y - 1) * self->width + x].as_uint = value;

    return NU_SUCCESS;
}
nu_result_t nusr_framebuffer_set_float(nusr_framebuffer_t *self, uint32_t x, uint32_t y, float value)
{
    self->pixels[(self->height - y - 1) * self->width + x].as_float = value;

    return NU_SUCCESS;
}
nu_result_t nusr_framebuffer_blend_uint(nusr_framebuffer_t *self, uint32_t x, uint32_t y, uint32_t value)
{
    uint32_t bvalue = self->pixels[(self->height - y - 1) * self->width + x].as_uint;

    uint32_t alpha = (value & 0xFF);
    uint32_t inv_alpha = 255 - alpha;

    bvalue >>= 8;
    value >>= 8;
    uint32_t rb = ((value & 0x00FF00FF) * alpha + (bvalue & 0x00FF00FF) * inv_alpha) & 0xFF00FF00;
    uint32_t g = ((value & 0x0000FF00) * alpha + (bvalue & 0x0000FF00) * inv_alpha) & 0x00FF0000;

    self->pixels[(self->height - y - 1) * self->width + x].as_uint = rb | g | 0xFF;

    return NU_SUCCESS;
}