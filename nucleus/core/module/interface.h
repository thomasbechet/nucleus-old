#ifndef NU_MODULE_INTERFACE_H
#define NU_MODULE_INTERFACE_H

#include "../common/common.h"

/* plugin interface */
#define NU_PLUGIN_INTERFACE_NAME        "nu_plugin_interface"
#define NU_PLUGIN_INTERFACE_LOADER_NAME "nu_plugin_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*late_update)(void);
    nu_result_t (*fixed_update)(void);
} nu_plugin_interface_t;

typedef nu_result_t (*nu_plugin_interface_loader_pfn_t)(nu_plugin_interface_t*, const char*);

/* task system interface */
#define NU_TASK_INTERFACE_NAME        "nu_task_interface"
#define NU_TASK_INTERFACE_LOADER_NAME "nu_task_get_interface"

NU_DECLARE_HANDLE(nu_task_handle_t);

typedef struct {
    void (*func)(void*, uint32_t, uint32_t);
    void *args;
} nu_job_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*create)(nu_task_handle_t *task);
    nu_result_t (*perform)(nu_task_handle_t task, nu_job_t *jobs, uint32_t count);
    nu_result_t (*wait)(nu_task_handle_t task);
    bool (*is_completed)(nu_task_handle_t task);
} nu_task_interface_t;

typedef nu_result_t (*nu_task_interface_loader_pfn_t)(nu_task_interface_t*);

/* window system interface */
#define NU_WINDOW_INTERFACE_NAME        "nu_window_interface"
#define NU_WINDOW_INTERFACE_LOADER_NAME "nu_window_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_size)(uint32_t*, uint32_t*);
} nu_window_interface_t;

typedef nu_result_t (*nu_window_interface_loader_pfn_t)(nu_window_interface_t*);

/* input system interface */
#define NU_INPUT_INTERFACE_NAME        "nu_input_interface"
#define NU_INPUT_INTERFACE_LOADER_NAME "nu_input_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_keyboard_state)(nu_button_state_t*, nu_keyboard_t);
    nu_result_t (*get_keyboard_text)(const char **text, uint32_t *length);
    nu_result_t (*get_mouse_state)(nu_button_state_t*, nu_mouse_t);
    nu_result_t (*get_mouse_motion)(nu_vec2_t);
    nu_result_t (*get_cursor_mode)(nu_cursor_mode_t*);
    nu_result_t (*set_cursor_mode)(nu_cursor_mode_t);
} nu_input_interface_t;

typedef nu_result_t (*nu_input_interface_loader_pfn_t)(nu_input_interface_t*);

/* renderer system interface */
#define NU_RENDERER_INTERFACE_NAME        "nu_renderer_interface"
#define NU_RENDERER_INTERFACE_LOADER_NAME "nu_renderer_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*render)(void);
} nu_renderer_interface_t;

typedef nu_result_t (*nu_renderer_interface_loader_pfn_t)(nu_renderer_interface_t*);

#endif