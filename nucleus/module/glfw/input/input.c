#include "input.h"

#include "../common/logger.h"
#include "../window/window.h"

#include <GLFW/glfw3.h>

static int glfw_keyboard_buttons[] = 
{
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,

    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,

    GLFW_KEY_ESCAPE,
    GLFW_KEY_SPACE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
    GLFW_KEY_UP,
    GLFW_KEY_DOWN
};

static int glfw_mouse_buttons[] = 
{
    GLFW_MOUSE_BUTTON_LEFT,
    GLFW_MOUSE_BUTTON_RIGHT,
    GLFW_MOUSE_BUTTON_MIDDLE,
    GLFW_MOUSE_BUTTON_1,
    GLFW_MOUSE_BUTTON_2,
    GLFW_MOUSE_BUTTON_3,
    GLFW_MOUSE_BUTTON_4,
    GLFW_MOUSE_BUTTON_5,
    GLFW_MOUSE_BUTTON_6
};

#define MAX_TEXT_SIZE 256

typedef struct {
    GLFWwindow *window;

    nu_cursor_mode_t cursor_mode;

    nu_vec2f_t mouse_motion;
    nu_vec2f_t mouse_scroll;
    nu_vec2f_t mouse_position;
    nu_vec2f_t mouse_old_position;
    
    char text[MAX_TEXT_SIZE];
    uint32_t text_size;
    
    nu_button_state_t keyboard_button_states[GLFW_KEY_LAST];
} nuglfw_input_data_t;

static nuglfw_input_data_t _data;

static void nuglfw_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key != GLFW_KEY_UNKNOWN) {
        if (action == GLFW_REPEAT) {
            _data.keyboard_button_states[key] |= NU_BUTTON_REPEATED;
        } else {
            bool was_pressed = (_data.keyboard_button_states[key] & NU_BUTTON_PRESSED);
            if (action == GLFW_PRESS) {
                _data.keyboard_button_states[key] |= NU_BUTTON_PRESSED;
                _data.keyboard_button_states[key] &= ~NU_BUTTON_RELEASED;
                if (!was_pressed) {
                    _data.keyboard_button_states[key] |= NU_BUTTON_JUST_PRESSED;
                }
            } else if (action == GLFW_RELEASE) {
                _data.keyboard_button_states[key] |= NU_BUTTON_RELEASED;
                _data.keyboard_button_states[key] &= ~NU_BUTTON_PRESSED;
                if (was_pressed) {
                    _data.keyboard_button_states[key] |= NU_BUTTON_JUST_RELEASED;
                }
            }
        }
    }
}
static void nuglfw_cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    _data.mouse_position[0] = (float)xpos;
    _data.mouse_position[1] = (float)ypos;
}
static void nuglfw_character_callback(GLFWwindow *window, uint32_t codepoint)
{
    if (_data.text_size >= MAX_TEXT_SIZE - 1) return;
    _data.text[_data.text_size++] = (char)codepoint;
}
static void nuglfw_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    _data.mouse_scroll[0] = (float)xoffset;
    _data.mouse_scroll[1] = (float)yoffset;
}

nu_result_t nuglfw_input_initialize(void)
{
    /* recover GLFWwindow */
    _data.window = (GLFWwindow*)nuglfw_get_window();

    /* initialize was pressed states */
    for (uint32_t i = 0; i < GLFW_KEY_LAST; i++) {
        _data.keyboard_button_states[i] = 0x0;
    }

    /* initialize keyboard text */
    _data.text_size = 0;

    /* setup default cursor mode */
    nuglfw_input_set_cursor_mode(nu_config_get().input.cursor_mode);

    if(glfwRawMouseMotionSupported()) {
        glfwSetInputMode(_data.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    /* setup callbacks */
    glfwSetKeyCallback(_data.window, nuglfw_keyboard_callback);
    glfwSetCursorPosCallback(_data.window, nuglfw_cursor_position_callback);
    glfwSetCharCallback(_data.window, nuglfw_character_callback);
    glfwSetScrollCallback(_data.window, nuglfw_mouse_scroll_callback);

    /* get initial mouse position */
    double xpos, ypos;
    glfwGetCursorPos(_data.window, &xpos, &ypos);
    _data.mouse_position[0] = (float)xpos;
    _data.mouse_position[1] = (float)ypos;
    nu_vec2f_copy(_data.mouse_position, _data.mouse_old_position);

    return NU_SUCCESS;
}
nu_result_t nuglfw_input_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_stop(void)
{
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_update(void)
{
    /* update mouse motion */
    nu_vec2f_sub(_data.mouse_position, _data.mouse_old_position, _data.mouse_motion);
    nu_vec2f_copy(_data.mouse_position, _data.mouse_old_position);

    return NU_SUCCESS;
}

nu_result_t nuglfw_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button)
{
    *state = _data.keyboard_button_states[glfw_keyboard_buttons[(uint32_t)button]];
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_keyboard_text(const char **text, uint32_t *length)
{
    *text = _data.text;
    *length = _data.text_size;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button)
{
    *state = (glfwGetMouseButton(_data.window, glfw_mouse_buttons[(uint32_t)button]) == GLFW_PRESS) ? NU_BUTTON_PRESSED : NU_BUTTON_RELEASED;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_motion(nu_vec2f_t motion)
{
    nu_vec2f_copy(_data.mouse_motion, motion);
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_scroll(nu_vec2f_t scroll)
{
    nu_vec2f_copy(_data.mouse_scroll, scroll);
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_cursor_mode(nu_cursor_mode_t *mode)
{
    *mode = _data.cursor_mode;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_set_cursor_mode(nu_cursor_mode_t mode)
{
    _data.cursor_mode = mode;
    if (mode == NU_CURSOR_MODE_NORMAL) {
        glfwSetInputMode(_data.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (mode == NU_CURSOR_MODE_HIDDEN) {
        glfwSetInputMode(_data.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else if (mode == NU_CURSOR_MODE_DISABLE) {
        glfwSetInputMode(_data.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    return NU_SUCCESS;
}

nu_result_t nuglfw_input_event_preupdate(void)
{
    /* reset keyboard text */
    _data.text_size = 0;
    memset(_data.text, 0, sizeof(char) * MAX_TEXT_SIZE);

    /* reset keyboard states */
    for (uint32_t i = 0; i < GLFW_KEY_LAST; i++) {
        _data.keyboard_button_states[i] &= ~NU_BUTTON_JUST_PRESSED;
        _data.keyboard_button_states[i] &= ~NU_BUTTON_JUST_RELEASED;
        _data.keyboard_button_states[i] &= ~NU_BUTTON_REPEATED;
    }

    /* reset scroll motion */
    nu_vec2f_zero(_data.mouse_scroll);

    return NU_SUCCESS;
}