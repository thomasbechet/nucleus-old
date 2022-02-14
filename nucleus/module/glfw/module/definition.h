/* Generated file: DO NOT EDIT ! */
#ifndef NUGLFW_MODULE_DEFINITION_H
#define NUGLFW_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>

/* module */
#define NUGLFW_MODULE_NAME "nucleus-glfw"
#define NUGLFW_MODULE_ID 1

/* interface */
#define NUGLFW_WINDOW_INTERFACE_NAME "nuglfw_window_interface"

typedef struct {
    nu_result_t (*get_required_instance_extensions)(const char***, uint32_t*);
    nu_result_t (*create_window_surface)(void*, void*, const void*);
    nu_result_t (*present_surface)(const nu_vec2u_t, void*);
    nu_result_t (*swap_buffers)(void);
} nuglfw_window_interface_t;


/* plugin */

#endif