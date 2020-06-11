#ifndef NUVK_SURFACE_H
#define NUVK_SURFACE_H

#include "../renderer.h"

nu_result_t nuvk_surface_create(void);
nu_result_t nuvk_surface_destroy(void);
VkSurfaceKHR nuvk_surface_get_handle(void);

#endif