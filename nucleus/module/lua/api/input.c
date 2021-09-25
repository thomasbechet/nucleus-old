#include <nucleus/module/lua/api/input.h>

#include <lauxlib.h>

static int Input_keyboard_pressed(lua_State *L)
{
    nu_keyboard_t key = (nu_keyboard_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_keyboard_state(key, &state);
    lua_pushboolean(L, state & NU_BUTTON_JUST_PRESSED ? 1 : 0);
    return 1;
}
static int Input_keyboard_released(lua_State *L)
{
    nu_keyboard_t key = (nu_keyboard_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_keyboard_state(key, &state);
    lua_pushboolean(L, state & NU_BUTTON_JUST_RELEASED ? 1 : 0);
    return 1;
}
static int Input_keyboard_is_pressed(lua_State *L)
{
    nu_keyboard_t key = (nu_keyboard_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_keyboard_state(key, &state);
    lua_pushboolean(L, state & NU_BUTTON_PRESSED ? 1 : 0);
    return 1;
}
static int Input_keyboard_is_released(lua_State *L)
{
    nu_keyboard_t key = (nu_keyboard_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_keyboard_state(key, &state);
    lua_pushboolean(L, state & NU_BUTTON_RELEASED ? 1 : 0);
    return 1;
}

static int Input_mouse_pressed(lua_State *L)
{
    nu_mouse_t mouse = (nu_mouse_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_mouse_state(mouse, &state);
    lua_pushboolean(L, state & NU_BUTTON_JUST_PRESSED ? 1 : 0);
    return 1;
}
static int Input_mouse_released(lua_State *L)
{
    nu_mouse_t mouse = (nu_mouse_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_mouse_state(mouse, &state);
    lua_pushboolean(L, state & NU_BUTTON_JUST_RELEASED ? 1 : 0);
    return 1;
}
static int Input_mouse_is_pressed(lua_State *L)
{
    nu_mouse_t mouse = (nu_mouse_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_mouse_state(mouse, &state);
    lua_pushboolean(L, state & NU_BUTTON_PRESSED ? 1 : 0);
    return 1;
}
static int Input_mouse_is_released(lua_State *L)
{
    nu_mouse_t mouse = (nu_mouse_t)luaL_checkinteger(L, -1);
    nu_button_state_t state;
    nu_input_get_mouse_state(mouse, &state);
    lua_pushboolean(L, state & NU_BUTTON_RELEASED ? 1 : 0);
    return 1;
}

static int Input_get_mouse_motion(lua_State *L)
{
    nu_vec2f_t motion;
    nu_input_get_mouse_motion(motion);
    lua_pushnumber(L, motion[0]);
    lua_pushnumber(L, motion[1]);
    return 2;
}
static int Input_get_mouse_scroll(lua_State *L)
{
    nu_vec2f_t scroll;
    nu_input_get_mouse_scroll(scroll);
    lua_pushnumber(L, scroll[0]);
    lua_pushnumber(L, scroll[1]);
    return 2;
}
static int Input_get_cursor_mode(lua_State *L)
{
    nu_cursor_mode_t mode;
    nu_input_get_cursor_mode(&mode);
    lua_pushinteger(L, (lua_Integer)mode);
    return 1;
}
static int Input_set_cursor_mode(lua_State *L)
{
    nu_cursor_mode_t mode = (nu_cursor_mode_t)luaL_checkinteger(L, -1);
    nu_input_set_cursor_mode(mode);
    return 0;
}

nu_result_t nulua_register_input_api(lua_State *L)
{
    static const struct luaL_Reg Input_methods[] = {
        {"keyboard_pressed",     Input_keyboard_pressed},
        {"keyboard_released",    Input_keyboard_released},
        {"keyboard_is_pressed",  Input_keyboard_is_pressed},
        {"keyboard_is_released", Input_keyboard_is_released},

        {"mouse_pressed",     Input_mouse_pressed},
        {"mouse_released",    Input_mouse_released},
        {"mouse_is_pressed",  Input_mouse_is_pressed},
        {"mouse_is_released", Input_mouse_is_released},

        {"get_mouse_motion", Input_get_mouse_motion},
        {"get_mouse_scroll", Input_get_mouse_scroll},
        {"get_cursor_mode",  Input_get_cursor_mode},
        {"set_cursor_mode",  Input_set_cursor_mode},

        {NULL, NULL}
    };

    luaL_newlib(L, Input_methods);

    lua_pushinteger(L, NU_KEYBOARD_A); lua_setfield(L, -2, "KEYBOARD_A");
    lua_pushinteger(L, NU_KEYBOARD_B); lua_setfield(L, -2, "KEYBOARD_B");
    lua_pushinteger(L, NU_KEYBOARD_C); lua_setfield(L, -2, "KEYBOARD_C");
    lua_pushinteger(L, NU_KEYBOARD_D); lua_setfield(L, -2, "KEYBOARD_D");
    lua_pushinteger(L, NU_KEYBOARD_E); lua_setfield(L, -2, "KEYBOARD_E");
    lua_pushinteger(L, NU_KEYBOARD_F); lua_setfield(L, -2, "KEYBOARD_F");
    lua_pushinteger(L, NU_KEYBOARD_G); lua_setfield(L, -2, "KEYBOARD_G");
    lua_pushinteger(L, NU_KEYBOARD_H); lua_setfield(L, -2, "KEYBOARD_H");
    lua_pushinteger(L, NU_KEYBOARD_I); lua_setfield(L, -2, "KEYBOARD_I");
    lua_pushinteger(L, NU_KEYBOARD_J); lua_setfield(L, -2, "KEYBOARD_J");
    lua_pushinteger(L, NU_KEYBOARD_K); lua_setfield(L, -2, "KEYBOARD_K");
    lua_pushinteger(L, NU_KEYBOARD_L); lua_setfield(L, -2, "KEYBOARD_L");
    lua_pushinteger(L, NU_KEYBOARD_M); lua_setfield(L, -2, "KEYBOARD_M");
    lua_pushinteger(L, NU_KEYBOARD_N); lua_setfield(L, -2, "KEYBOARD_N");
    lua_pushinteger(L, NU_KEYBOARD_O); lua_setfield(L, -2, "KEYBOARD_O");
    lua_pushinteger(L, NU_KEYBOARD_P); lua_setfield(L, -2, "KEYBOARD_P");
    lua_pushinteger(L, NU_KEYBOARD_Q); lua_setfield(L, -2, "KEYBOARD_Q");
    lua_pushinteger(L, NU_KEYBOARD_R); lua_setfield(L, -2, "KEYBOARD_R");
    lua_pushinteger(L, NU_KEYBOARD_S); lua_setfield(L, -2, "KEYBOARD_S");
    lua_pushinteger(L, NU_KEYBOARD_T); lua_setfield(L, -2, "KEYBOARD_T");
    lua_pushinteger(L, NU_KEYBOARD_U); lua_setfield(L, -2, "KEYBOARD_U");
    lua_pushinteger(L, NU_KEYBOARD_V); lua_setfield(L, -2, "KEYBOARD_V");
    lua_pushinteger(L, NU_KEYBOARD_W); lua_setfield(L, -2, "KEYBOARD_W");
    lua_pushinteger(L, NU_KEYBOARD_X); lua_setfield(L, -2, "KEYBOARD_X");
    lua_pushinteger(L, NU_KEYBOARD_Y); lua_setfield(L, -2, "KEYBOARD_Y");
    lua_pushinteger(L, NU_KEYBOARD_Z); lua_setfield(L, -2, "KEYBOARD_Z");

    lua_pushinteger(L, NU_KEYBOARD_F1); lua_setfield(L, -2, "KEYBOARD_F1");
    lua_pushinteger(L, NU_KEYBOARD_F2); lua_setfield(L, -2, "KEYBOARD_F2");
    lua_pushinteger(L, NU_KEYBOARD_F3); lua_setfield(L, -2, "KEYBOARD_F3");
    lua_pushinteger(L, NU_KEYBOARD_F4); lua_setfield(L, -2, "KEYBOARD_F4");
    lua_pushinteger(L, NU_KEYBOARD_F5); lua_setfield(L, -2, "KEYBOARD_F5");
    lua_pushinteger(L, NU_KEYBOARD_F6); lua_setfield(L, -2, "KEYBOARD_F6");
    lua_pushinteger(L, NU_KEYBOARD_F7); lua_setfield(L, -2, "KEYBOARD_F7");
    lua_pushinteger(L, NU_KEYBOARD_F8); lua_setfield(L, -2, "KEYBOARD_F8");
    lua_pushinteger(L, NU_KEYBOARD_F9); lua_setfield(L, -2, "KEYBOARD_F9");
    lua_pushinteger(L, NU_KEYBOARD_F10); lua_setfield(L, -2, "KEYBOARD_F10");
    lua_pushinteger(L, NU_KEYBOARD_F11); lua_setfield(L, -2, "KEYBOARD_F11");
    lua_pushinteger(L, NU_KEYBOARD_F12); lua_setfield(L, -2, "KEYBOARD_F12");

    lua_pushinteger(L, NU_KEYBOARD_NUM0); lua_setfield(L, -2, "KEYBOARD_NUM0");
    lua_pushinteger(L, NU_KEYBOARD_NUM1); lua_setfield(L, -2, "KEYBOARD_NUM1");
    lua_pushinteger(L, NU_KEYBOARD_NUM2); lua_setfield(L, -2, "KEYBOARD_NUM2");
    lua_pushinteger(L, NU_KEYBOARD_NUM3); lua_setfield(L, -2, "KEYBOARD_NUM3");
    lua_pushinteger(L, NU_KEYBOARD_NUM4); lua_setfield(L, -2, "KEYBOARD_NUM4");
    lua_pushinteger(L, NU_KEYBOARD_NUM5); lua_setfield(L, -2, "KEYBOARD_NUM5");
    lua_pushinteger(L, NU_KEYBOARD_NUM6); lua_setfield(L, -2, "KEYBOARD_NUM6");
    lua_pushinteger(L, NU_KEYBOARD_NUM7); lua_setfield(L, -2, "KEYBOARD_NUM7");
    lua_pushinteger(L, NU_KEYBOARD_NUM8); lua_setfield(L, -2, "KEYBOARD_NUM8");
    lua_pushinteger(L, NU_KEYBOARD_NUM9); lua_setfield(L, -2, "KEYBOARD_NUM9");

    lua_pushinteger(L, NU_KEYBOARD_ESCAPE);    lua_setfield(L, -2, "KEYBOARD_ESCAPE");
    lua_pushinteger(L, NU_KEYBOARD_SPACE);     lua_setfield(L, -2, "KEYBOARD_SPACE");
    lua_pushinteger(L, NU_KEYBOARD_ENTER);     lua_setfield(L, -2, "KEYBOARD_ENTER");
    lua_pushinteger(L, NU_KEYBOARD_TAB);       lua_setfield(L, -2, "KEYBOARD_TAB");
    lua_pushinteger(L, NU_KEYBOARD_BACKSPACE); lua_setfield(L, -2, "KEYBOARD_BACKSPACE");
    lua_pushinteger(L, NU_KEYBOARD_LSHIFT);    lua_setfield(L, -2, "KEYBOARD_LSHIFT");
    lua_pushinteger(L, NU_KEYBOARD_LCONTROL);  lua_setfield(L, -2, "KEYBOARD_LCONTROL");
    lua_pushinteger(L, NU_KEYBOARD_LEFT);      lua_setfield(L, -2, "KEYBOARD_LEFT");
    lua_pushinteger(L, NU_KEYBOARD_RIGHT);     lua_setfield(L, -2, "KEYBOARD_RIGHT");
    lua_pushinteger(L, NU_KEYBOARD_UP);        lua_setfield(L, -2, "KEYBOARD_UP");
    lua_pushinteger(L, NU_KEYBOARD_DOWN);      lua_setfield(L, -2, "KEYBOARD_DOWN");

    lua_pushinteger(L, NU_MOUSE_LEFT);    lua_setfield(L, -2, "MOUSE_LEFT");
    lua_pushinteger(L, NU_MOUSE_RIGHT);   lua_setfield(L, -2, "MOUSE_RIGHT");
    lua_pushinteger(L, NU_MOUSE_MIDDLE);  lua_setfield(L, -2, "MOUSE_MIDDLE");
    lua_pushinteger(L, NU_MOUSE_BUTTON1); lua_setfield(L, -2, "MOUSE_BUTTON1");
    lua_pushinteger(L, NU_MOUSE_BUTTON2); lua_setfield(L, -2, "MOUSE_BUTTON2");
    lua_pushinteger(L, NU_MOUSE_BUTTON3); lua_setfield(L, -2, "MOUSE_BUTTON3");
    lua_pushinteger(L, NU_MOUSE_BUTTON4); lua_setfield(L, -2, "MOUSE_BUTTON4");
    lua_pushinteger(L, NU_MOUSE_BUTTON5); lua_setfield(L, -2, "MOUSE_BUTTON5");
    lua_pushinteger(L, NU_MOUSE_BUTTON6); lua_setfield(L, -2, "MOUSE_BUTTON6");

    lua_pushinteger(L, NU_CURSOR_MODE_NORMAL);  lua_setfield(L, -2, "CURSOR_MODE_NORMAL");
    lua_pushinteger(L, NU_CURSOR_MODE_HIDDEN);  lua_setfield(L, -2, "CURSOR_MODE_HIDDEN");
    lua_pushinteger(L, NU_CURSOR_MODE_DISABLE); lua_setfield(L, -2, "CURSOR_MODE_DISABLE");

    lua_setglobal(L, "Input");

    return NU_SUCCESS;
}