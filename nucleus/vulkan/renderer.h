#ifndef NUVK_RENDERER_H
#define NUVK_RENDERER_H

#include "../core/nucleus_core.h"
#include "../glfw/module/interface.h"

#include <vulkan/vulkan.h>

nu_result_t nuvk_renderer_initialize(void);
nu_result_t nuvk_renderer_terminate(void);
nu_result_t nuvk_renderer_render(void);

#endif