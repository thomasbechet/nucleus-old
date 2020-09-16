#ifndef NUGLFW_MODULE_INTERFACE_H
#define NUGLFW_MODULE_INTERFACE_H

#include "../../../core/nucleus.h"

/* module */
#define NUGLFW_MODULE_NAME "nucleus-glfw"
#define NUGLFW_MODULE_ID 0x4

/* glfw system */
#define NUGLFW_WINDOW_INTERFACE_NAME        "nuglfw_window_interface"
#define NUGLFW_WINDOW_INTERFACE_LOADER_NAME "nuglfw_window_get_interface"

typedef struct {
    const char **(*get_required_instance_extensions)(uint32_t*);
    nu_result_t (*create_window_surface)(nu_ptr_t, nu_ptr_t);
    nu_result_t (*present_surface)(uint32_t, uint32_t, void*);
} nuglfw_window_interface_t;

typedef nu_result_t (*nuglfw_window_interface_loader_pfn_t)(nuglfw_window_interface_t*);

#endif