#include <nucleus/core/system/input/api.h>

nu_cursor_mode_t nu_cursor_mode_deserialize(const char *mode, nu_cursor_mode_t default_mode)
{
    if (NU_MATCH(mode, "normal")) {
        return NU_CURSOR_MODE_NORMAL;
    } else if (NU_MATCH(mode, "hidden")) {
        return NU_CURSOR_MODE_HIDDEN;
    } else if (NU_MATCH(mode, "disable")) {
        return NU_CURSOR_MODE_DISABLE;
    } else {
        return default_mode;
    }
}
const char *nu_cursor_mode_serialize(nu_cursor_mode_t mode)
{
    switch (mode)
    {
    case NU_CURSOR_MODE_NORMAL: return "normal";
    case NU_CURSOR_MODE_HIDDEN: return "hidden";
    case NU_CURSOR_MODE_DISABLE: return "disable";
    default: return NULL;
    }
}