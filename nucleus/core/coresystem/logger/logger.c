#include <nucleus/core/coresystem/logger/logger.h>

#define NU_CORE_LOGGER_NAME "[CORE] "

#ifdef NU_PLATFORM_WINDOWS
#include <windows.h>
#endif

nu_result_t nu_logger_initialize(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_logger_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_logger_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_logger_stop(void)
{
    return NU_SUCCESS;
}

void nu_log(nu_severity_t severity, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(severity, format, args);
    va_end(args);
}
void nu_vlog(nu_severity_t severity, const char *format, va_list args)
{
#if defined(NU_PLATFORM_WINDOWS)
    switch (severity) {
        case NU_INFO:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
            break;
        case NU_WARNING:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
            break;
        case NU_ERROR:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            break;
        case NU_FATAL:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            break;
    }
    vfprintf(stdout, format, args);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#elif defined(NU_PLATFORM_UNIX)
    switch (severity) {
        case NU_INFO:
            fprintf(stdout, "\x1B[34m");
            break;
        case NU_WARNING:
            fprintf(stdout, "\033[0;33m");
            break;
        case NU_ERROR:
            fprintf(stdout, "\x1B[31m");
            break;
        case NU_FATAL:
            fprintf(stdout, "\x1B[31m");
            break;
    }
    vfprintf(stdout, format, args);
    fprintf(stdout, "\x1B[0m");
#endif
}
void nu_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_INFO, format, args);
    va_end(args);
}
void nu_vinfo(const char *format, va_list args)
{
    nu_vlog(NU_INFO, format, args);
}
void nu_warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_WARNING, format, args);
    va_end(args);
}
void nu_vwarning(const char *format, va_list args)
{
    nu_vlog(NU_WARNING, format, args);
}
void nu_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_ERROR, format, args);
    va_end(args);
}
void nu_verror(const char *format, va_list args)
{
    nu_vlog(NU_ERROR, format, args);
}
void nu_fatal(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_FATAL, format, args);
    va_end(args);
}
void nu_vfatal(const char *format, va_list args)
{
    nu_vlog(NU_FATAL, format, args);
}

void nu_core_log(nu_severity_t severity, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_core_vlog(severity, format, args);
    va_end(args);
}
void nu_core_vlog(nu_severity_t severity, const char *format, va_list args)
{
/* This function must not use any other system */
#if defined(NU_PLATFORM_WINDOWS)
    switch (severity) {
        case NU_INFO:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
            break;
        case NU_WARNING:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
            break;
        case NU_ERROR:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            break;
        case NU_FATAL:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            break;
    }
    fprintf(stdout, NU_CORE_LOGGER_NAME);
    vfprintf(stdout, format, args);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#elif defined(NU_PLATFORM_UNIX)
    switch (severity) {
        case NU_INFO:
            fprintf(stdout, "\x1B[34m");
            break;
        case NU_WARNING:
            fprintf(stdout, "\033[0;33m");
            break;
        case NU_ERROR:
            fprintf(stdout, "\x1B[31m");
            break;
        case NU_FATAL:
            fprintf(stdout, "\x1B[31m");
            break;
    }
    fprintf(stdout, NU_CORE_LOGGER_NAME);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\x1B[0m");
#endif
}