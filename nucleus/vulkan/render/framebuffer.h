#ifndef NUVK_FRAMEBUFFER_H
#define NUVK_FRAMEBUFFER_H

#include "../renderer.h"

nu_result_t nuvk_framebuffer_create(void);
nu_result_t nuvk_framebuffer_destroy(void);

const VkFramebuffer *nuvk_framebuffer_get_swapchain(uint32_t *count);

#endif