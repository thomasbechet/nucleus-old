#ifndef NU_SYSTEM_WINDOW_H
#define NU_SYSTEM_WINDOW_H

#include "../module/module.h"
#include "../module/interface.h"

typedef enum {
    NU_WINDOW_API_NONE = 0,
    NU_WINDOW_API_GLFW = 1
} nu_window_api_t;

/* private system functions */
nu_result_t nu_system_window_load(nu_window_api_t api);
nu_result_t nu_system_window_unload(void);
nu_result_t nu_system_window_update(void);

/* public system functions */
NU_API const nu_module_t *nu_system_window_get_module(void);

/* public window functions */
NU_API nu_result_t nu_window_get_size(uint32_t *width, uint32_t *height);
NU_API nu_result_t nu_window_set_size(const uint32_t *size);
NU_API nu_result_t nu_window_set_title(const char *title);

#endif