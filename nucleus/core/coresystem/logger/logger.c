#include <nucleus/core/coresystem/logger/logger.h>

#include <nucleus/core/config/config.h>

#ifdef NU_PLATFORM_WINDOWS
#include <windows.h>
#endif

typedef struct {
    nu_file_t log_file;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_logger_initialize(void)
{
    _system.log_file = NULL;
    if (nu_config_get().logger.enable_log_file) {
        nu_path_t path, filename;
        nu_path_allocate_cstr(nu_config_get().logger.log_file_directory, &path);
        nu_path_allocate_cstr("nucleus.log", &filename);
        nu_path_join(&path, filename);
        if (nu_file_open(path, NU_IO_MODE_WRITE, &_system.log_file) != NU_SUCCESS) {
            nu_error(NU_LOGGER_NAME, "Failed to open log file: %s.", nu_path_get_cstr(path));
            return NU_FAILURE;
        }
        nu_path_free(path);
        nu_path_free(filename);
    }

    return NU_SUCCESS;
}
nu_result_t nu_logger_terminate(void)
{
    if (_system.log_file) {
        nu_file_close(_system.log_file);
        _system.log_file = NULL;
    }

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

void nu_log(nu_severity_t severity, const char *id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(severity, id, format, args);
    va_end(args);
}
void nu_vlog(nu_severity_t severity, const char *id, const char *format, va_list args)
{
    if (_system.log_file) {
        nu_file_write_printf(_system.log_file, "[%s] ", id);
        nu_file_write_vprintf(_system.log_file, format, args);
        nu_file_write_printf(_system.log_file, "\n");
    }

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
    fprintf(stdout, "[%s] ", id);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
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
    fprintf(stdout, "[%s] ", id);
    vfprintf(stdout, format, args);
    printf(stdout, "\n");
    fprintf(stdout, "\x1B[0m");
#endif
}
void nu_info(const char *id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_INFO, id, format, args);
    va_end(args);
}
void nu_vinfo(const char *id, const char *format, va_list args)
{
    nu_vlog(NU_INFO, id, format, args);
}
void nu_warning(const char *id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_WARNING, id, format, args);
    va_end(args);
}
void nu_vwarning(const char *id, const char *format, va_list args)
{
    nu_vlog(NU_WARNING, id, format, args);
}
void nu_error(const char *id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_ERROR, id, format, args);
    va_end(args);
}
void nu_verror(const char *id, const char *format, va_list args)
{
    nu_vlog(NU_ERROR, id, format, args);
}
void nu_fatal(const char *id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_FATAL, id, format, args);
    va_end(args);
}
void nu_vfatal(const char *id, const char *format, va_list args)
{
    nu_vlog(NU_FATAL, id, format, args);
}