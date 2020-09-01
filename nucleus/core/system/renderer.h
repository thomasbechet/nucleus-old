#ifndef NU_SYSTEM_RENDERER_H
#define NU_SYSTEM_RENDERER_H

#include "../module/module.h"

typedef enum {
    NU_RENDERER_API_NONE       = 0,
    NU_RENDERER_API_SOFTRAST   = 1,
    NU_RENDERER_API_VULKAN     = 2,
    NU_RENDERER_API_OPENGL     = 3
} nu_renderer_api_t;

typedef struct {
    bool vertical_synchronization;
} nu_renderer_create_info_t;

/* private system functions */
nu_result_t nu_system_renderer_initialize(void);
nu_result_t nu_system_renderer_terminate(void);
nu_result_t nu_system_renderer_render(void);

/* public system functions */
NU_API nu_module_handle_t nu_system_renderer_get_module_handle(void);

/* public renderer functions */

#endif