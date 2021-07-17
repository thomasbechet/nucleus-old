#ifndef NU_LOGGER_H
#define NU_LOGGER_H

#include <nucleus/core/logger/interface.h>

nu_result_t nu_logger_initialize(void);
nu_result_t nu_logger_terminate(void);
nu_result_t nu_logger_start(void);
nu_result_t nu_logger_stop(void);

void nu_core_log(nu_severity_t severity, const char *format, ...);
void nu_core_vlog(nu_severity_t severity, const char *format, va_list args);

#endif