#ifndef NU_API_LOGGER_H
#define NU_API_LOGGER_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>

NU_DECLARE_HANDLE(nu_logger_t);

typedef enum {
    NU_INFO,
    NU_WARNING,
    NU_ERROR,
    NU_FATAL
} nu_severity_t;

typedef struct {
    nu_logger_t (*create)(const char *name);
    nu_logger_t (*get)(const char *name);
    void (*destroy)(nu_logger_t logger);
    void (*log)(nu_logger_t logger, nu_severity_t severity, const char *format, ...);
    void (*vlog)(nu_logger_t logger, nu_severity_t severity, const char *format, va_list args);
    void (*info)(nu_logger_t logger, const char *format, ...);
    void (*vinfo)(nu_logger_t logger, const char *format, va_list args);
    void (*warning)(nu_logger_t logger, const char *format, ...);
    void (*vwarning)(nu_logger_t logger, const char *format, va_list args);
    void (*error)(nu_logger_t logger, const char *format, ...);
    void (*verror)(nu_logger_t logger, const char *format, va_list args);
    void (*fatal)(nu_logger_t logger, const char *format, ...);
    void (*vfatal)(nu_logger_t logger, const char *format, va_list args);
} nu_logger_api_t;

#endif