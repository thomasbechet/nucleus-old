#ifndef NU_SYSTEM_INPUT_H
#define NU_SYSTEM_INPUT_H

#include "../module/module.h"

typedef enum {
    NU_INPUT_API_NONE = 0,
    NU_INPUT_API_GLFW = 1
} nu_input_api_t;

typedef enum {
    NU_INPUT_CURSOR_MODE_NORMAL,
    NU_INPUT_CURSOR_MODE_HIDDEN,
    NU_INPUT_CURSOR_MODE_DISABLE
} nu_input_cursor_mode_t;

/* private system functions */
nu_result_t nu_system_input_load(void);
nu_result_t nu_system_input_unload(void);
nu_result_t nu_system_input_update(void);

/* public system functions */
NU_API const nu_module_t *nu_system_input_get_module(void);

/* public input functions */
NU_API nu_result_t nu_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button);
NU_API nu_result_t nu_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button);
NU_API nu_result_t nu_input_get_mouse_motion(vec2 motion);

#endif