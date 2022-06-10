#ifndef NU_INPUT_API_H
#define NU_INPUT_API_H

#include <nucleus/core/api/input.h>

NU_API nu_cursor_mode_t nu_cursor_mode_deserialize(const char *mode, nu_cursor_mode_t default_mode);
NU_API const char *nu_cursor_mode_serialize(nu_cursor_mode_t mode);

#endif