#include "viewport.h"

#include "../common/config.h"

typedef struct {
    nusr_renderbuffer_t renderbuffer;
} nusr_viewport_data_t;

static nusr_viewport_data_t _data;

nu_result_t nusr_viewport_initialize(void)
{
    /* creating renderbuffer */
    uint32_t default_width, default_height;
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_WIDTH, &default_width, 640);
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_HEIGHT, &default_height, 360);
    nusr_renderbuffer_create(&_data.renderbuffer, default_width, default_height);

    return NU_SUCCESS;
}
nu_result_t nusr_viewport_terminate(void)
{
    /* free renderbuffer */
    nusr_renderbuffer_destroy(&_data.renderbuffer);
    
    return NU_SUCCESS;
}
nu_result_t nusr_viewport_get_renderbuffer(nusr_renderbuffer_t **renderbuffer)
{
    *renderbuffer = &_data.renderbuffer;
    
    return NU_SUCCESS;
}
nu_result_t nusr_viewport_set_size(uint32_t width, uint32_t height)
{
    /* recreate renderbuffer */
    nusr_renderbuffer_destroy(&_data.renderbuffer);
    nusr_renderbuffer_create(&_data.renderbuffer, width, height);

    return NU_SUCCESS;
}
nu_result_t nusr_viewport_get_size(uint32_t *width, uint32_t *height)
{
    *width = _data.renderbuffer.color_buffer.width;
    *height = _data.renderbuffer.color_buffer.height;

    return NU_SUCCESS;
}