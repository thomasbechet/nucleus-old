#ifndef NU_WINDOW_API_H
#define NU_WINDOW_API_H

#include <nucleus/core/api/window.h>

NU_API nu_window_mode_t nu_window_mode_deserialize(const char *mode, nu_window_mode_t default_mode);
NU_API const char *nu_window_mode_serialize(nu_window_mode_t mode);

#endif