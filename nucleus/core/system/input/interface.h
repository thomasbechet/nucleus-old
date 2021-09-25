#ifndef NU_INPUT_INTERFACE_H
#define NU_INPUT_INTERFACE_H

#include <nucleus/core/coresystem/module/interface.h>

#define NU_INPUT_INTERFACE_NAME "nu_input_interface"

typedef enum {
    NU_BUTTON_PRESSED       = 0x1 << 0,
    NU_BUTTON_RELEASED      = 0x1 << 1,
    NU_BUTTON_JUST_PRESSED  = 0x1 << 2,
    NU_BUTTON_JUST_RELEASED = 0x1 << 3,
    NU_BUTTON_REPEATED      = 0x1 << 4
} nu_button_state_t;

typedef enum {
    NU_BUTTON_KEYBOARD,
    NU_BUTTON_MOUSE
} nu_button_type_t;

typedef enum {
    NU_KEYBOARD_A = 0,
    NU_KEYBOARD_B = NU_KEYBOARD_A + 1,
    NU_KEYBOARD_C = NU_KEYBOARD_B + 1,
    NU_KEYBOARD_D = NU_KEYBOARD_C + 1,
    NU_KEYBOARD_E = NU_KEYBOARD_D + 1,
    NU_KEYBOARD_F = NU_KEYBOARD_E + 1,
    NU_KEYBOARD_G = NU_KEYBOARD_F + 1,
    NU_KEYBOARD_H = NU_KEYBOARD_G + 1,
    NU_KEYBOARD_I = NU_KEYBOARD_H + 1,
    NU_KEYBOARD_J = NU_KEYBOARD_I + 1,
    NU_KEYBOARD_K = NU_KEYBOARD_J + 1,
    NU_KEYBOARD_L = NU_KEYBOARD_K + 1,
    NU_KEYBOARD_M = NU_KEYBOARD_L + 1,
    NU_KEYBOARD_N = NU_KEYBOARD_M + 1,
    NU_KEYBOARD_O = NU_KEYBOARD_N + 1,
    NU_KEYBOARD_P = NU_KEYBOARD_O + 1,
    NU_KEYBOARD_Q = NU_KEYBOARD_P + 1,
    NU_KEYBOARD_R = NU_KEYBOARD_Q + 1,
    NU_KEYBOARD_S = NU_KEYBOARD_R + 1,
    NU_KEYBOARD_T = NU_KEYBOARD_S + 1,
    NU_KEYBOARD_U = NU_KEYBOARD_T + 1,
    NU_KEYBOARD_V = NU_KEYBOARD_U + 1,
    NU_KEYBOARD_W = NU_KEYBOARD_V + 1,
    NU_KEYBOARD_X = NU_KEYBOARD_W + 1,
    NU_KEYBOARD_Y = NU_KEYBOARD_X + 1,
    NU_KEYBOARD_Z = NU_KEYBOARD_Y + 1,

    NU_KEYBOARD_F1 = NU_KEYBOARD_Z + 1,
    NU_KEYBOARD_F2 = NU_KEYBOARD_F1 + 1,
    NU_KEYBOARD_F3 = NU_KEYBOARD_F2 + 1,
    NU_KEYBOARD_F4 = NU_KEYBOARD_F3 + 1,
    NU_KEYBOARD_F5 = NU_KEYBOARD_F4 + 1,
    NU_KEYBOARD_F6 = NU_KEYBOARD_F5 + 1,
    NU_KEYBOARD_F7 = NU_KEYBOARD_F6 + 1,
    NU_KEYBOARD_F8 = NU_KEYBOARD_F7 + 1,
    NU_KEYBOARD_F9 = NU_KEYBOARD_F8 + 1,
    NU_KEYBOARD_F10 = NU_KEYBOARD_F9 + 1,
    NU_KEYBOARD_F11 = NU_KEYBOARD_F10 + 1,
    NU_KEYBOARD_F12 = NU_KEYBOARD_F11 + 1,

    NU_KEYBOARD_NUM0 = NU_KEYBOARD_F12 + 1,
    NU_KEYBOARD_NUM1 = NU_KEYBOARD_NUM0 + 1,
    NU_KEYBOARD_NUM2 = NU_KEYBOARD_NUM1 + 1,
    NU_KEYBOARD_NUM3 = NU_KEYBOARD_NUM2 + 1,
    NU_KEYBOARD_NUM4 = NU_KEYBOARD_NUM3 + 1,
    NU_KEYBOARD_NUM5 = NU_KEYBOARD_NUM4 + 1,
    NU_KEYBOARD_NUM6 = NU_KEYBOARD_NUM5 + 1,
    NU_KEYBOARD_NUM7 = NU_KEYBOARD_NUM6 + 1,
    NU_KEYBOARD_NUM8 = NU_KEYBOARD_NUM7 + 1,
    NU_KEYBOARD_NUM9 = NU_KEYBOARD_NUM8 + 1,

    NU_KEYBOARD_ESCAPE = NU_KEYBOARD_NUM9 + 1,
    NU_KEYBOARD_SPACE = NU_KEYBOARD_ESCAPE + 1,
    NU_KEYBOARD_ENTER = NU_KEYBOARD_SPACE + 1,
    NU_KEYBOARD_TAB = NU_KEYBOARD_ENTER + 1,
    NU_KEYBOARD_BACKSPACE = NU_KEYBOARD_TAB + 1,
    NU_KEYBOARD_LSHIFT = NU_KEYBOARD_BACKSPACE + 1,
    NU_KEYBOARD_LCONTROL = NU_KEYBOARD_LSHIFT + 1,
    NU_KEYBOARD_LEFT = NU_KEYBOARD_LCONTROL + 1,
    NU_KEYBOARD_RIGHT = NU_KEYBOARD_LEFT + 1,
    NU_KEYBOARD_UP = NU_KEYBOARD_RIGHT + 1,
    NU_KEYBOARD_DOWN = NU_KEYBOARD_UP + 1
} nu_keyboard_t;

typedef enum {
    NU_MOUSE_LEFT = 0,
    NU_MOUSE_RIGHT = NU_MOUSE_LEFT + 1,
    NU_MOUSE_MIDDLE = NU_MOUSE_RIGHT + 1,
    NU_MOUSE_BUTTON1 = NU_MOUSE_MIDDLE + 1,
    NU_MOUSE_BUTTON2 = NU_MOUSE_BUTTON1 + 1,
    NU_MOUSE_BUTTON3 = NU_MOUSE_BUTTON2 + 1,
    NU_MOUSE_BUTTON4 = NU_MOUSE_BUTTON3 + 1,
    NU_MOUSE_BUTTON5 = NU_MOUSE_BUTTON4 + 1,
    NU_MOUSE_BUTTON6 = NU_MOUSE_BUTTON5 + 1
} nu_mouse_t;

typedef enum {
    NU_CURSOR_MODE_NORMAL,
    NU_CURSOR_MODE_HIDDEN,
    NU_CURSOR_MODE_DISABLE
} nu_cursor_mode_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*start)(void);
    nu_result_t (*stop)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_keyboard_state)(nu_keyboard_t, nu_button_state_t*);
    nu_result_t (*get_keyboard_text)(const char **text, uint32_t *length);
    nu_result_t (*get_mouse_state)(nu_mouse_t, nu_button_state_t*);
    nu_result_t (*get_mouse_motion)(nu_vec2f_t);
    nu_result_t (*get_mouse_scroll)(nu_vec2f_t);
    nu_result_t (*get_cursor_mode)(nu_cursor_mode_t*);
    nu_result_t (*set_cursor_mode)(nu_cursor_mode_t);
} nu_input_interface_t;

typedef enum {
    NU_INPUT_API_NONE = 0,
    NU_INPUT_API_GLFW = 1
} nu_input_api_t;

NU_API nu_module_t nu_input_get_module(void);

NU_API nu_result_t nu_input_get_keyboard_state(nu_keyboard_t button, nu_button_state_t *state);
NU_API nu_result_t nu_input_get_keyboard_text(const char **text, uint32_t *length);
NU_API nu_result_t nu_input_get_mouse_state(nu_mouse_t button, nu_button_state_t *state);
NU_API nu_result_t nu_input_get_mouse_motion(nu_vec2f_t motion);
NU_API nu_result_t nu_input_get_mouse_scroll(nu_vec2f_t scroll);
NU_API nu_result_t nu_input_get_cursor_mode(nu_cursor_mode_t *mode);
NU_API nu_result_t nu_input_set_cursor_mode(nu_cursor_mode_t mode);

#endif