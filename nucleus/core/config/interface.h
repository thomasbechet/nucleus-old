#ifndef NU_CONFIG_INTERFACE_H
#define NU_CONFIG_INTERFACE_H

#include <nucleus/core/system/input/interface.h>
#include <nucleus/core/system/renderer/interface.h>
#include <nucleus/core/system/task/interface.h>
#include <nucleus/core/system/window/interface.h>

#define NU_CONFIG_CONTEXT_SECTION       "context"
#define NU_CONFIG_CONTEXT_VERSION_MAJOR "version_major"
#define NU_CONFIG_CONTEXT_VERSION_MINOR "version_minor"
#define NU_CONFIG_CONTEXT_VERSION_PATCH "version_patch"
#define NU_CONFIG_CONTEXT_LOG_CONFIG    "log_config"

#define NU_CONFIG_LOGGER_SECTION              "logger"
#define NU_CONFIG_LOGGER_ENABLE_LOG_FILE      "enable_log_file"
#define NU_CONFIG_LOGGER_ENABLE_CORE_LOG_FILE "log_file"
#define NU_CONFIG_LOGGER_LOG_FILE_DIRECTORY   "log_file_directory"

#define NU_CONFIG_TASK_SECTION "task"
#define NU_CONFIG_TASK_API     "api"

#define NU_CONFIG_WINDOW_SECTION "window"
#define NU_CONFIG_WINDOW_API     "api"
#define NU_CONFIG_WINDOW_MODE    "mode"
#define NU_CONFIG_WINDOW_WIDTH   "width"
#define NU_CONFIG_WINDOW_HEIGHT  "height"
#define NU_CONFIG_WINDOW_VSYNC   "vertical_synchronization"

#define NU_CONFIG_INPUT_SECTION     "input"
#define NU_CONFIG_INPUT_API         "api"
#define NU_CONFIG_INPUT_CURSOR_MODE "cursor_mode"

#define NU_CONFIG_RENDERER_SECTION "renderer"
#define NU_CONFIG_RENDERER_API     "api"

typedef struct {
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    bool log_config;
} nu_config_context_t;

typedef struct {
    bool enable_log_file;
    bool enable_core_log_file;
    const char *log_file_directory;
} nu_config_logger_t;

typedef struct {
    nu_task_api_t api;
} nu_config_task_t;

typedef struct {
    nu_window_api_t api;
    nu_window_mode_t mode;
    uint32_t width;
    uint32_t height;
    bool vsync;
} nu_config_window_t;

typedef struct {
    nu_input_api_t api;
    nu_cursor_mode_t cursor_mode;
} nu_config_input_t;

typedef struct {
    nu_renderer_api_t api;
} nu_config_renderer_t;

typedef struct {
    nu_config_context_t context;
    nu_config_logger_t logger;
    nu_config_task_t task;
    nu_config_window_t window;
    nu_config_input_t input;
    nu_config_renderer_t renderer;
} nu_config_t;

typedef nu_result_t (*nu_config_callback_pfn_t)(nu_config_t*);

NU_API nu_config_t nu_config_get(void);
NU_API nu_result_t nu_config_get_cstr(const char *section, const char *name, const char *default_value, const char **value);
NU_API nu_result_t nu_config_get_int(const char *section, const char *name, int32_t default_value, int32_t *value);
NU_API nu_result_t nu_config_get_uint(const char *section, const char *name, uint32_t default_value, uint32_t *value);
NU_API nu_result_t nu_config_get_bool(const char *section, const char *name, bool default_value, bool *value);
NU_API nu_result_t nu_config_get_float(const char *section, const char *name, float default_value, float *value);

NU_API nu_result_t nu_config_log(void);

#endif