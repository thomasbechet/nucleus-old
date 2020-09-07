#include "viewport.h"

typedef struct {
    nusr_renderbuffer_t renderbuffer;
} nusr_viewport_data_t;

static nusr_viewport_data_t _data;

nu_result_t nusr_viewport_initialize(void)
{
    /* creating framebuffer */
    uint32_t default_width, default_height;
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_WIDTH, &default_width, 640);
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_HEIGHT, &default_height, 360);
    nusr_framebuffer_create(&_data.color_buffer, default_width, default_height);
    nusr_framebuffer_create(&_data.depth_buffer, default_width, default_height);

    return NU_SUCCESS;
}
nu_result_t nusr_viewport_terminate(void)
{
    
    return NU_SUCCESS;
}
nu_result_t nusr_viewport_get_renderbuffer(nusr_renderbuffer_t *renderbuffer)
{
    
    return NU_SUCCESS;
}