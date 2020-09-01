#ifndef NU_SYSTEM_WINDOW_H
#define NU_SYSTEM_WINDOW_H

#include "../module/module.h"

typedef enum {
    NU_WINDOW_API_NONE = 0,
    NU_WINDOW_API_GLFW = 1
} nu_window_api_t;

typedef enum {
    NU_WINDOW_MODE_FULLSCREEN,
    NU_WINDOW_MODE_WINDOWED,
    NU_WINDOW_MODE_BORDERLESS
} nu_window_mode_t;

/* private system functions */
nu_result_t nu_system_window_initialize(void);
nu_result_t nu_system_window_terminate(void);
nu_result_t nu_system_window_update(void);

/* public system functions */
NU_API nu_module_handle_t nu_system_window_get_module_handle(void);

/* public window functions */
NU_API nu_result_t nu_window_get_size(uint32_t *width, uint32_t *height);
NU_API nu_result_t nu_window_set_size(const uint32_t *size);
NU_API nu_result_t nu_window_set_title(const char *title);

#endif