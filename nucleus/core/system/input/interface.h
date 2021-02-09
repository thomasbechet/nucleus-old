#ifndef NU_INPUT_INTERFACE_H
#define NU_INPUT_INTERFACE_H

#include "../../module/interface.h"

#define NU_INPUT_INTERFACE_NAME "nu_input_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*start)(void);
    nu_result_t (*stop)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_keyboard_state)(nu_button_state_t*, nu_keyboard_t);
    nu_result_t (*get_keyboard_text)(const char **text, uint32_t *length);
    nu_result_t (*get_mouse_state)(nu_button_state_t*, nu_mouse_t);
    nu_result_t (*get_mouse_motion)(nu_vec2f_t);
    nu_result_t (*get_mouse_scroll)(nu_vec2f_t);
    nu_result_t (*get_cursor_mode)(nu_cursor_mode_t*);
    nu_result_t (*set_cursor_mode)(nu_cursor_mode_t);
} nu_input_interface_t;

typedef enum {
    NU_INPUT_API_NONE = 0,
    NU_INPUT_API_GLFW = 1
} nu_input_api_t;

NU_API nu_module_handle_t nu_input_get_module_handle(void);

NU_API nu_result_t nu_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button);
NU_API nu_result_t nu_input_get_keyboard_text(const char **text, uint32_t *length);
NU_API nu_result_t nu_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button);
NU_API nu_result_t nu_input_get_mouse_motion(nu_vec2f_t motion);
NU_API nu_result_t nu_input_get_mouse_scroll(nu_vec2f_t scroll);
NU_API nu_result_t nu_input_get_cursor_mode(nu_cursor_mode_t *mode);
NU_API nu_result_t nu_input_set_cursor_mode(nu_cursor_mode_t mode);

#endif