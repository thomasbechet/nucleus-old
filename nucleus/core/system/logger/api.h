#ifndef NU_LOGGER_API_H
#define NU_LOGGER_API_H

#include <nucleus/core/api/logger.h>

NU_API nu_logger_t nu_logger_create(const char *name);
NU_API nu_logger_t nu_logger_get(const char *name);
NU_API void nu_log(nu_logger_t logger, nu_severity_t severity, const char *format, ...);
NU_API void nu_vlog(nu_logger_t logger, nu_severity_t severity, const char *format, va_list args);
NU_API void nu_info(nu_logger_t logger, const char *format, ...);
NU_API void nu_vinfo(nu_logger_t logger, const char *format, va_list args);
NU_API void nu_warning(nu_logger_t logger, const char *format, ...);
NU_API void nu_vwarning(nu_logger_t logger, const char *format, va_list args);
NU_API void nu_error(nu_logger_t logger, const char *format, ...);
NU_API void nu_verror(nu_logger_t logger, const char *format, va_list args);
NU_API void nu_fatal(nu_logger_t logger, const char *format, ...);
NU_API void nu_vfatal(nu_logger_t logger, const char *format, va_list args);

#endif