#include "renderbuffer.h"

nu_result_t nusr_renderbuffer_create(nusr_renderbuffer_t *self, uint32_t width, uint32_t height)
{
    nusr_framebuffer_create(&self->color_buffer, width, height);
    nusr_framebuffer_create(&self->depth_buffer, width, height);

    return NU_SUCCESS;
}
nu_result_t nusr_renderbuffer_destroy(nusr_renderbuffer_t *self)
{
    nusr_framebuffer_destroy(&self->color_buffer);
    nusr_framebuffer_destroy(&self->depth_buffer);

    return NU_SUCCESS;
}