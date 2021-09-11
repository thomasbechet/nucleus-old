#ifndef NU_LOGGER_INTERFACE_H
#define NU_LOGGER_INTERFACE_H

#include <nucleus/core/utility/all.h>

/* logger name */
#define NU_LOGGER_NAME "CORE"

/* error */
#define _NU_S(x) #x
#define _NU_S_(x) _NU_S(x)
#define _NU_S__LINE__ _NU_S_(__LINE__)

#define _NU_CHECK(check, action, file, line, message, ...) \
    if (!(check)) { \
        nu_error( "Invalid check: "file":"line"\n"); \
        nu_error(message, ##__VA_ARGS__); \
        action; \
    }

#define NU_CHECK(check, action, message, ...) \
    _NU_CHECK(check, action, __FILE__, _NU_S__LINE__, message, ##__VA_ARGS__)

typedef enum {
    NU_INFO     = 0x1 << 0,
    NU_WARNING  = 0x1 << 1,
    NU_ERROR    = 0x1 << 2,
    NU_FATAL    = 0x1 << 3
} nu_severity_t;

NU_API void nu_log(nu_severity_t severity, const char *id, const char *format, ...);
NU_API void nu_vlog(nu_severity_t severity, const char *id, const char *format, va_list args);
NU_API void nu_info(const char *id, const char *format, ...);
NU_API void nu_vinfo(const char *id, const char *format, va_list args);
NU_API void nu_warning(const char *id, const char *format, ...);
NU_API void nu_vwarning(const char *id, const char *format, va_list args);
NU_API void nu_error(const char *id, const char *format, ...);
NU_API void nu_verror(const char *id, const char *format, va_list args);
NU_API void nu_fatal(const char *id, const char *format, ...);
NU_API void nu_vfatal(const char *id, const char *format, va_list args);

#endif