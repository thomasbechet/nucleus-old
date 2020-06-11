#ifndef NU_MODULE_INTERFACE_H
#define NU_MODULE_INTERFACE_H

#include "../common/common.h"

#define NU_WINDOW_INTERFACE "nu_window"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_size)(uint32_t*, uint32_t*);
} nu_window_interface_t;

NU_API nu_result_t nu_window_interface_get_required_functions(const char ***functions, uint32_t *count);

#define NU_RENDERER_INTERFACE "nu_renderer"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*render)(void);
} nu_renderer_interface_t;

NU_API nu_result_t nu_renderer_interface_get_required_functions(const char ***functions, uint32_t *count);

#endif