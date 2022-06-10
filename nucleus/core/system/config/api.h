#ifndef NU_CONFIG_API_H
#define NU_CONFIG_API_H

#include <nucleus/core/api/config.h>

NU_API nu_result_t nu_config_load(const char *path);
NU_API const char *nu_config_get_cstr(const char *section, const char *name, const char *default_value);
NU_API int32_t nu_config_get_int(const char *section, const char *name, int32_t default_value);
NU_API uint32_t nu_config_get_uint(const char *section, const char *name, uint32_t default_value);
NU_API bool nu_config_get_bool(const char *section, const char *name, bool default_value);
NU_API float nu_config_get_float(const char *section, const char *name, float default_value);
NU_API void nu_config_log(void);

#endif