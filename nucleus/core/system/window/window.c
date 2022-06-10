#include <nucleus/core/system/window/api.h>

nu_window_mode_t nu_window_mode_deserialize(const char *mode, nu_window_mode_t default_mode)
{
    if (NU_MATCH(mode, "fullscreen")) {
        return NU_WINDOW_MODE_FULLSCREEN;
    } else if (NU_MATCH(mode, "windowed")) {
        return NU_WINDOW_MODE_WINDOWED;
    } else if (NU_MATCH(mode, "borderless")) {
        return NU_WINDOW_MODE_BORDERLESS;
    } else {
        return default_mode;
    }
}
const char *nu_window_mode_serialize(nu_window_mode_t mode)
{
    switch (mode)
    {
    case NU_WINDOW_MODE_FULLSCREEN: return "fullscreen";
    case NU_WINDOW_MODE_WINDOWED: return "windowed";
    case NU_WINDOW_MODE_BORDERLESS: return "borderless";
    default: return NULL;
    }
}