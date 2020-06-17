#ifndef NUVK_RENDER_PASS_H
#define NUVK_RENDER_PASS_H

#include "../renderer.h"

nu_result_t nuvk_render_pass_create(void);
nu_result_t nuvk_render_pass_destroy(void);

VkRenderPass nuvk_render_pass_get_handle(void);

#endif 