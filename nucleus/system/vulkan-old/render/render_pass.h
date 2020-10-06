#ifndef NUVK_RENDER_PASS_H
#define NUVK_RENDER_PASS_H

#include "../renderer.h"

nu_result_t nuvk_render_pass_create_default(VkRenderPass *renderpass);
nu_result_t nuvk_render_pass_destroy(VkRenderPass renderpass);

#endif 