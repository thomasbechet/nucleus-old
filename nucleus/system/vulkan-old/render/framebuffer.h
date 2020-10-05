#ifndef NUVK_FRAMEBUFFER_H
#define NUVK_FRAMEBUFFER_H

#include "../renderer.h"

nu_result_t nuvk_framebuffer_create(
    VkFramebuffer *framebuffer,
    VkImageView *images,
    uint32_t image_count,
    VkExtent2D extent,
    VkRenderPass render_pass
);
nu_result_t nuvk_framebuffer_destroy(VkFramebuffer framebuffer);

#endif