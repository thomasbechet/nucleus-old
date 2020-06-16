#ifndef NUGLFW_MODULE_INTERFACE_H
#define NUGLFW_MODULE_INTERFACE_H

#include "../../core/nucleus_core.h"

#define NUGLFW_MODULE_NAME "nucleus-glfw"
#define NUGLFW_MODULE_ID 0x4

#define NUGLFW_WINDOW_INTERFACE_NAME        "nuglfw_window_interface"
#define NUGLFW_WINDOW_INTERFACE_LOADER_NAME "nuglfw_window_interface_loader"

typedef struct {
    const char **(*get_required_instance_extensions)(uint32_t*);
    nu_result_t (*create_window_surface)(nu_ptr_t, nu_ptr_t);
} nuglfw_window_interface_t;

typedef nu_result_t (*nuglfw_window_interface_loader_pfn_t)(nuglfw_window_interface_t*);

#endif