#include "framebuffer.h"

nu_result_t nurz_framebuffer_create(nurz_framebuffer_t *self, uint32_t width, uint32_t height)
{
    self->pixels = (uint32_t*)nu_malloc(sizeof(uint32_t) * width * height);
    self->width = width;
    self->height = height;

    return NU_SUCCESS;
}
nu_result_t nurz_framebuffer_destroy(nurz_framebuffer_t *self)
{
    nu_free(self->pixels);
    self->pixels = NULL;
    self->width = 0;
    self->height = 0;

    return NU_SUCCESS;
}
nu_result_t nurz_framebuffer_clear(nurz_framebuffer_t *self, uint32_t color)
{
    memset(self->pixels, color, sizeof(uint32_t) * self->width * self->height);

    return NU_SUCCESS;
}
nu_result_t nurz_framebuffer_set_rgb(nurz_framebuffer_t *self,
    uint32_t x, uint32_t y,
    float fr, float fg, float fb
)
{
    uint32_t r = (uint32_t)(fr * 255.0f);
    uint32_t g = (uint32_t)(fg * 255.0f);
    uint32_t b = (uint32_t)(fb * 255.0f);
    self->pixels[y * self->width + x] = ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8);

    return NU_SUCCESS;
}