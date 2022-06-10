#ifndef NU_API_WINDOW_H
#define NU_API_WINDOW_H

#include <nucleus/core/api/module.h>
#include <nucleus/core/utility/math/math.h>

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
} nu_window_system_api_t;

typedef struct {
    void (*set_size)(const nu_vec2u_t size);
    void (*get_size)(nu_vec2u_t size);
    void (*set_title)(const char *title);
    void (*set_mode)(nu_window_mode_t mode);
} nu_window_api_t;

typedef enum {
    NU_WINDOW_MODULE_NONE = 0,
    NU_WINDOW_MODULE_GLFW = 1
} nu_window_module_t;

NU_API nu_module_t nu_window_get_module(void);

#endif