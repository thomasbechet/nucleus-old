#ifndef NUGLFW_CONTEXT_H
#define NUGLFW_CONTEXT_H

#include "../core/nucleus_core.h"

/* window interface */
nu_result_t nuglfw_window_initialize(void);
nu_result_t nuglfw_window_terminate(void);
nu_result_t nuglfw_window_update(void);
nu_result_t nuglfw_window_get_size(uint32_t *width, uint32_t *height);

/* glfw window interface */
const char **nuglfw_get_required_instance_extensions(uint32_t *count);
nu_result_t nuglfw_create_window_surface(nu_ptr_t instance, nu_ptr_t surface);
nu_result_t nuglfw_present_surface(
    uint32_t width,
    uint32_t height,
    void *pixels
);

#endif