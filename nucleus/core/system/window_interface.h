#ifndef NU_SYSTEM_WINDOW_INTERFACE_H
#define NU_SYSTEM_WINDOW_INTERFACE_H

#include "../common/common.h"

/* window system interface */
#define NU_WINDOW_INTERFACE_NAME        "nu_window_interface"
#define NU_WINDOW_INTERFACE_LOADER_NAME "nu_window_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*set_size)(uint32_t, uint32_t);
    nu_result_t (*get_size)(uint32_t*, uint32_t*);
    nu_result_t (*set_title)(const char*);
} nu_window_interface_t;

typedef nu_result_t (*nu_window_interface_loader_pfn_t)(nu_window_interface_t*);

#endif