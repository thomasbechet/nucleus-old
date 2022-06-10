#ifndef NU_API_CONFIG_H
#define NU_API_CONFIG_H

#include <nucleus/core/api/window.h>
#include <nucleus/core/api/input.h>
#include <nucleus/core/api/renderer.h>
#include <nucleus/core/api/task.h>

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
#define NU_CONFIG_TASK_MODULE  "module"

#define NU_CONFIG_WINDOW_SECTION "window"
#define NU_CONFIG_WINDOW_MODULE  "module"
#define NU_CONFIG_WINDOW_MODE    "mode"
#define NU_CONFIG_WINDOW_WIDTH   "width"
#define NU_CONFIG_WINDOW_HEIGHT  "height"
#define NU_CONFIG_WINDOW_VSYNC   "vertical_synchronization"

#define NU_CONFIG_INPUT_SECTION     "input"
#define NU_CONFIG_INPUT_MODULE      "module"
#define NU_CONFIG_INPUT_CURSOR_MODE "cursor_mode"

#define NU_CONFIG_RENDERER_SECTION "renderer"
#define NU_CONFIG_RENDERER_MODULE  "module"

typedef struct {
    nu_result_t (*load)(const char *path);
    const char *(*get_cstr)(const char *section, const char *name, const char *default_value);
    int32_t (*get_int)(const char *section, const char *name, int32_t default_value);
    uint32_t (*get_uint)(const char *section, const char *name, uint32_t default_value);
    bool (*get_bool)(const char *section, const char *name, bool default_value);
    float (*get_float)(const char *section, const char *name, float default_value);
    void (*log)(void);
} nu_config_api_t;

#endif