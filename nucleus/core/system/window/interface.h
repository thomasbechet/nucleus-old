#ifndef NU_WINDOW_INTERFACE_H
#define NU_WINDOW_INTERFACE_H

#include <nucleus/core/coresystem/module/interface.h>

#define NU_WINDOW_INTERFACE_NAME "nu_window_interface"

typedef enum {
    NU_WINDOW_MODE_FULLSCREEN,
    NU_WINDOW_MODE_WINDOWED,
    NU_WINDOW_MODE_BORDERLESS
} nu_window_mode_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*start)(void);
    nu_result_t (*stop)(void);
    nu_result_t (*update)(void);
    nu_result_t (*set_size)(const nu_vec2u_t);
    nu_result_t (*get_size)(nu_vec2u_t);
    nu_result_t (*set_title)(const char*);
    nu_result_t (*set_mode)(nu_window_mode_t);
} nu_window_interface_t;

typedef enum {
    NU_WINDOW_API_NONE = 0,
    NU_WINDOW_API_GLFW = 1
} nu_window_api_t;

NU_API nu_module_t nu_window_get_module(void);

NU_API nu_result_t nu_window_set_size(const nu_vec2u_t size);
NU_API nu_result_t nu_window_get_size(nu_vec2u_t size);
NU_API nu_result_t nu_window_set_title(const char *title);
NU_API nu_result_t nu_window_set_mode(nu_window_mode_t mode);

#endif