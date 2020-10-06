#ifndef NUVK_SURFACE_H
#define NUVK_SURFACE_H

#include "context.h"

nu_result_t nuvk_surface_create(VkSurfaceKHR *surface, VkInstance instance);
nu_result_t nuvk_surface_destroy(VkSurfaceKHR surface, VkInstance instance);

#endif