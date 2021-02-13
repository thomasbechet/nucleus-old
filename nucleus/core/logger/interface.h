#ifndef NU_LOGGER_INTERFACE_H
#define NU_LOGGER_INTERFACE_H

#include <nucleus/core/common/common.h>

typedef enum {
    NU_INFO     = 0x1 << 0,
    NU_WARNING  = 0x1 << 1,
    NU_ERROR    = 0x1 << 2,
    NU_FATAL    = 0x1 << 3
} nu_severity_t;

NU_API void nu_log(nu_severity_t severity, const char *format, ...);
NU_API void nu_vlog(nu_severity_t severity, const char *format, va_list args);
NU_API void nu_info(const char *format, ...);
NU_API void nu_vinfo(const char *format, va_list args);
NU_API void nu_warning(const char *format, ...);
NU_API void nu_vwarning(const char *format, va_list args);
NU_API void nu_error(const char *format, ...);
NU_API void nu_verror(const char *format, va_list args);
NU_API void nu_fatal(const char *format, ...);
NU_API void nu_vfatal(const char *format, va_list args);

#endif