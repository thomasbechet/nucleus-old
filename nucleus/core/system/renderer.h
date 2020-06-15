#ifndef NU_SYSTEM_RENDERER_H
#define NU_SYSTEM_RENDERER_H

#include "../module/module.h"

typedef enum {
    NU_RENDERER_API_NONE   = 0,
    NU_RENDERER_API_VULKAN = 1,
    NU_RENDERER_API_OPENGL = 2
} nu_renderer_api_t;

/* private system functions */
nu_result_t nu_system_renderer_load(nu_renderer_api_t api);
nu_result_t nu_system_renderer_unload(void);
nu_result_t nu_system_renderer_render(void);

/* public system functions */
NU_API const nu_module_t *nu_system_renderer_get_module(void);

/* public renderer functions */

#endif