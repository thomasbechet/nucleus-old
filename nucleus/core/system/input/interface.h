#ifndef NU_SYSTEM_INPUT_INTERFACE_H
#define NU_SYSTEM_INPUT_INTERFACE_H

#include "../../common/common.h"

/* input system interface */
#define NU_INPUT_INTERFACE_LOADER_NAME "nu_input_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_keyboard_state)(nu_button_state_t*, nu_keyboard_t);
    nu_result_t (*get_keyboard_text)(const char **text, uint32_t *length);
    nu_result_t (*get_mouse_state)(nu_button_state_t*, nu_mouse_t);
    nu_result_t (*get_mouse_motion)(nu_vec2f_t);
    nu_result_t (*get_mouse_scroll)(nu_vec2f_t);
    nu_result_t (*get_cursor_mode)(nu_cursor_mode_t*);
    nu_result_t (*set_cursor_mode)(nu_cursor_mode_t);
} nu_input_interface_t;

typedef nu_result_t (*nu_input_interface_loader_pfn_t)(nu_input_interface_t*);

#endif