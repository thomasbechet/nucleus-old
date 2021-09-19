#include <nucleus/module/glfw/input/input.h>

#include <nucleus/module/glfw/common/logger.h>
#include <nucleus/module/glfw/window/window.h>

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
    nu_button_state_t mouse_button_states[GLFW_MOUSE_BUTTON_LAST];
} nuglfw_module_data_t;

static nuglfw_module_data_t _module;

static void nuglfw_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key != GLFW_KEY_UNKNOWN) {
        if (action == GLFW_REPEAT) {
            _module.keyboard_button_states[key] |= NU_BUTTON_REPEATED;
        } else {
            bool was_pressed = (_module.keyboard_button_states[key] & NU_BUTTON_PRESSED);
            if (action == GLFW_PRESS) {
                _module.keyboard_button_states[key] |= NU_BUTTON_PRESSED;
                _module.keyboard_button_states[key] &= ~NU_BUTTON_RELEASED;
                if (!was_pressed) {
                    _module.keyboard_button_states[key] |= NU_BUTTON_JUST_PRESSED;
                }
            } else if (action == GLFW_RELEASE) {
                _module.keyboard_button_states[key] |= NU_BUTTON_RELEASED;
                _module.keyboard_button_states[key] &= ~NU_BUTTON_PRESSED;
                if (was_pressed) {
                    _module.keyboard_button_states[key] |= NU_BUTTON_JUST_RELEASED;
                }
            }
        }
    }
}
static void nuglfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mode)
{
    if (action == GLFW_REPEAT) {
        _module.mouse_button_states[button] |= NU_BUTTON_REPEATED;
    } else {
        bool was_pressed = (_module.mouse_button_states[button] & NU_BUTTON_PRESSED);
        if (action == GLFW_PRESS) {
            _module.mouse_button_states[button] |= NU_BUTTON_PRESSED;
            _module.mouse_button_states[button] &= ~NU_BUTTON_RELEASED;
            if (!was_pressed) {
                _module.mouse_button_states[button] |= NU_BUTTON_JUST_PRESSED;
            }
        } else if (action == GLFW_RELEASE) {
            _module.mouse_button_states[button] |= NU_BUTTON_RELEASED;
            _module.mouse_button_states[button] &= ~NU_BUTTON_PRESSED;
            if (was_pressed) {
                _module.mouse_button_states[button] |= NU_BUTTON_JUST_RELEASED;
            }
        }
    }
}
static void nuglfw_cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    _module.mouse_position[0] = (float)xpos;
    _module.mouse_position[1] = (float)ypos;
}
static void nuglfw_character_callback(GLFWwindow *window, uint32_t codepoint)
{
    if (_module.text_size >= MAX_TEXT_SIZE - 1) return;
    _module.text[_module.text_size++] = (char)codepoint;
}
static void nuglfw_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    _module.mouse_scroll[0] = (float)xoffset;
    _module.mouse_scroll[1] = (float)yoffset;
}

nu_result_t nuglfw_input_initialize(void)
{
    /* recover GLFWwindow */
    _module.window = (GLFWwindow*)nuglfw_get_window();

    /* initialize was pressed states */
    for (uint32_t i = 0; i < GLFW_KEY_LAST; i++) {
        _module.keyboard_button_states[i] = 0x0;
    }
    for (uint32_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
        _module.mouse_button_states[i] = 0x0;
    }

    /* initialize keyboard text */
    _module.text_size = 0;

    /* setup default cursor mode */
    nuglfw_input_set_cursor_mode(nu_config_get().input.cursor_mode);

    if(glfwRawMouseMotionSupported()) {
        glfwSetInputMode(_module.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    /* setup callbacks */
    glfwSetKeyCallback(_module.window, nuglfw_keyboard_callback);
    glfwSetMouseButtonCallback(_module.window, nuglfw_mouse_button_callback);
    glfwSetCursorPosCallback(_module.window, nuglfw_cursor_position_callback);
    glfwSetCharCallback(_module.window, nuglfw_character_callback);
    glfwSetScrollCallback(_module.window, nuglfw_mouse_scroll_callback);

    /* get initial mouse position */
    double xpos, ypos;
    glfwGetCursorPos(_module.window, &xpos, &ypos);
    _module.mouse_position[0] = (float)xpos;
    _module.mouse_position[1] = (float)ypos;
    nu_vec2f_copy(_module.mouse_position, _module.mouse_old_position);

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
    nu_vec2f_sub(_module.mouse_position, _module.mouse_old_position, _module.mouse_motion);
    nu_vec2f_copy(_module.mouse_position, _module.mouse_old_position);

    return NU_SUCCESS;
}

nu_result_t nuglfw_input_get_keyboard_state(nu_keyboard_t button, nu_button_state_t *state)
{
    *state = _module.keyboard_button_states[glfw_keyboard_buttons[(uint32_t)button]];
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_keyboard_text(const char **text, uint32_t *length)
{
    *text = _module.text;
    *length = _module.text_size;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_state(nu_mouse_t button, nu_button_state_t *state)
{
    *state = _module.mouse_button_states[glfw_mouse_buttons[(uint32_t)button]];
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_motion(nu_vec2f_t motion)
{
    nu_vec2f_copy(_module.mouse_motion, motion);
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_scroll(nu_vec2f_t scroll)
{
    nu_vec2f_copy(_module.mouse_scroll, scroll);
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_cursor_mode(nu_cursor_mode_t *mode)
{
    *mode = _module.cursor_mode;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_set_cursor_mode(nu_cursor_mode_t mode)
{
    _module.cursor_mode = mode;
    if (mode == NU_CURSOR_MODE_NORMAL) {
        glfwSetInputMode(_module.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (mode == NU_CURSOR_MODE_HIDDEN) {
        glfwSetInputMode(_module.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else if (mode == NU_CURSOR_MODE_DISABLE) {
        glfwSetInputMode(_module.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    return NU_SUCCESS;
}

nu_result_t nuglfw_input_event_preupdate(void)
{
    /* reset keyboard text */
    _module.text_size = 0;
    memset(_module.text, 0, sizeof(char) * MAX_TEXT_SIZE);

    /* reset keyboard states */
    for (uint32_t i = 0; i < GLFW_KEY_LAST; i++) {
        _module.keyboard_button_states[i] &= ~NU_BUTTON_JUST_PRESSED;
        _module.keyboard_button_states[i] &= ~NU_BUTTON_JUST_RELEASED;
        _module.keyboard_button_states[i] &= ~NU_BUTTON_REPEATED;
    }

    /* reset scroll motion */
    nu_vec2f_zero(_module.mouse_scroll);

    return NU_SUCCESS;
}