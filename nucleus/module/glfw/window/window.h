#ifndef NUGLFW_WINDOW_H
#define NUGLFW_WINDOW_H

#include <nucleus/nucleus.h>

/* window interface */
nu_result_t nuglfw_window_initialize(void);
nu_result_t nuglfw_window_terminate(void);
nu_result_t nuglfw_window_update(void);

nu_result_t nuglfw_window_set_size(const nu_vec2u_t size);
nu_result_t nuglfw_window_get_size(nu_vec2u_t size);
nu_result_t nuglfw_window_set_title(const char *title);
nu_result_t nuglfw_window_set_mode(nu_window_mode_t mode);

/* glfw window interface */
const char **nuglfw_get_required_instance_extensions(uint32_t *count);
nu_result_t nuglfw_create_window_surface(nu_ptr_t instance, nu_ptr_t surface);
nu_result_t nuglfw_present_surface(
    const nu_vec2u_t size,
    void *pixels
);
nu_result_t nuglfw_swap_buffers(void);

/* utility functions */
nu_ptr_t nuglfw_get_window(void);

#endif