#include <nucleus/core/system/logger/logger.h>

#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/system/config/api.h>
#include <nucleus/core/system/module/module.h>
#include <nucleus/core/system/module/api.h>

#ifdef NU_PLATFORM_WINDOWS
#include <windows.h>
#endif

#define MAX_NAME_LENGTH 64

typedef struct {
    char name[MAX_NAME_LENGTH];
} nu_logger_data_t;

typedef struct {
    nu_file_t log_file;
    nu_vector(nu_logger_data_t) loggers;
    nu_logger_t core_logger;
} nu_state_t;

static nu_state_t s_state;

static const nu_logger_api_t s_logger_api = {
    .create   = nu_logger_create,
    .get      = NULL,
    .destroy  = NULL,
    .log      = nu_log,
    .vlog     = nu_vlog,
    .info     = nu_info,
    .vinfo    = nu_vinfo,
    .warning  = nu_warning,
    .vwarning = nu_vwarning,
    .error    = nu_error,
    .verror   = nu_verror,
    .fatal    = nu_fatal,
    .vfatal   = nu_vfatal
};

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_logger_initialize(void)
{
    // Allocate loggers
    s_state.loggers = nu_vector_allocate(nu_allocator_get_core(), nu_logger_data_t);

    // Create core allocator
    nu_logger_data_t *logger = nu_vector_push(&s_state.loggers);
    nu_snprintf(logger->name, MAX_NAME_LENGTH, "CORE");
    NU_HANDLE_SET_ID(s_state.core_logger, 0);

    // Configure log file
    s_state.log_file = NU_NULL_HANDLE;
    // s_state.log_file = NU_NULL_HANDLE;
    // if (nu_config_get()->logger.enable_log_file) {      
    //     nu_result_t result = NU_SUCCESS;  
    //     nu_string_t path = nu_string_allocate(nu_allocator_get_core(), nu_config_get()->logger.log_file_directory);
    //     nu_string_resolve_path(&path);
    //     if (!nu_string_is_directory(path)) result = NU_FAILURE;
    //     NU_CHECK(result == NU_SUCCESS, goto cleanup0, nu_logger_get_core(), "Log file directory is not a directory: %s.", path);
    //     nu_string_append(&path, "nucleus.log");
    //     s_state.log_file = nu_file_open(path, NU_IO_MODE_WRITE);
    //     NU_CHECK(result == NU_SUCCESS, goto cleanup0, nu_logger_get_core(), "Failed to open log file: %s.", path);
    
    // cleanup0:
    //     nu_string_free(path);
    //     return result;
    // }

    return NU_SUCCESS;
}
nu_result_t nu_logger_terminate(void)
{
    if (s_state.log_file) {
        nu_file_close(s_state.log_file);
        s_state.log_file = NULL;
    }

    // Free resources
    nu_vector_free(s_state.loggers);

    return NU_SUCCESS;
}
nu_result_t nu_logger_register_api(void)
{
    return nu_module_register_api(nu_module_get_core(), nu_logger_api_t, &s_logger_api);
}
nu_logger_t nu_logger_get_core(void)
{
    return s_state.core_logger;
}

// +--------------------------------------------------------------------------+
// |                               PUBLIC API                                 |
// +--------------------------------------------------------------------------+

nu_logger_t nu_logger_create(const char *name)
{
    nu_logger_t handle; NU_HANDLE_SET_ID(handle, nu_vector_size(s_state.loggers));
    nu_logger_data_t *logger = nu_vector_push(&s_state.loggers);
    strncpy(logger->name, name, MAX_NAME_LENGTH);
    return handle;
}
nu_logger_t nu_logger_get(const char *name)
{
    (void)name;
    return NU_NULL_HANDLE;
}
void nu_log(nu_logger_t logger, nu_severity_t severity, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(logger, severity, format, args);
    va_end(args);
}
void nu_vlog(nu_logger_t logger, nu_severity_t severity, const char *format, va_list args)
{
    // Get logger info
    nu_logger_data_t *data = &s_state.loggers[NU_HANDLE_GET_ID(logger)];

    const char *severity_name;
    switch (severity) {
        case NU_INFO: severity_name = "INFO"; break;
        case NU_WARNING: severity_name = "WARNING"; break;
        case NU_ERROR: severity_name = "ERROR"; break;
        case NU_FATAL: severity_name = "FATAL"; break;
    }

    if (s_state.log_file) {
        nu_file_write_printf(s_state.log_file, "[%s][%s] ", data->name, severity_name);
        nu_file_write_vprintf(s_state.log_file, format, args);
        nu_file_write_printf(s_state.log_file, "\n");
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
    fprintf(stdout, "[%s][%s] ", data->name, severity_name);
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
    fprintf(stdout, "[%s] ", data->name);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    fprintf(stdout, "\x1B[0m");
#endif
}
void nu_info(nu_logger_t logger, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(logger, NU_INFO, format, args);
    va_end(args);
}
void nu_vinfo(nu_logger_t logger, const char *format, va_list args)
{
    nu_vlog(logger, NU_INFO, format, args);
}
void nu_warning(nu_logger_t logger, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(logger, NU_WARNING, format, args);
    va_end(args);
}
void nu_vwarning(nu_logger_t logger, const char *format, va_list args)
{
    nu_vlog(logger, NU_WARNING, format, args);
}
void nu_error(nu_logger_t logger, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(logger, NU_ERROR, format, args);
    va_end(args);
}
void nu_verror(nu_logger_t logger, const char *format, va_list args)
{
    nu_vlog(logger, NU_ERROR, format, args);
}
void nu_fatal(nu_logger_t logger, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(logger, NU_FATAL, format, args);
    va_end(args);
}
void nu_vfatal(nu_logger_t logger, const char *format, va_list args)
{
    nu_vlog(logger, NU_FATAL, format, args);
}