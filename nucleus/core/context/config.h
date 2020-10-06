#ifndef NU_CONFIG_H
#define NU_CONFIG_H

#include "../system/window/window.h"
#include "../system/input/input.h"
#include "../system/renderer/renderer.h"

#define NU_CONFIG_CONTEXT_SECTION       "context"
#define NU_CONFIG_CONTEXT_VERSION_MAJOR "version_major"
#define NU_CONFIG_CONTEXT_VERSION_MINOR "version_minor"
#define NU_CONFIG_CONTEXT_VERSION_PATCH "version_patch"
#define NU_CONFIG_CONTEXT_LOG_CONFIG    "log_config"

#define NU_CONFIG_WINDOW_SECTION        "window"
#define NU_CONFIG_WINDOW_API            "api"
#define NU_CONFIG_WINDOW_MODE           "mode"
#define NU_CONFIG_WINDOW_WIDTH          "width"
#define NU_CONFIG_WINDOW_HEIGHT         "height"
#define NU_CONFIG_WINDOW_VSYNC          "vertical_synchronization"

#define NU_CONFIG_INPUT_SECTION         "input"
#define NU_CONFIG_INPUT_API             "api"
#define NU_CONFIG_INPUT_CURSOR_MODE     "cursor_mode"

#define NU_CONFIG_RENDERER_SECTION      "renderer"
#define NU_CONFIG_RENDERER_API          "api"

typedef struct {
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    bool log_config;
} nu_config_context_t;

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
    nu_config_window_t window;
    nu_config_input_t input;
    nu_config_renderer_t renderer;
} nu_config_t;

typedef nu_result_t (*nu_config_callback_pfn_t)(nu_config_t*);

nu_result_t nu_config_load(nu_config_callback_pfn_t callback);
nu_result_t nu_config_unload(void);

NU_API nu_config_t nu_config_get(void);
NU_API nu_result_t nu_config_log(void);

NU_API nu_result_t nu_config_get_int(const char *section, const char *name, int32_t *value, int32_t default_value);
NU_API nu_result_t nu_config_get_uint(const char *section, const char *name, uint32_t *value, uint32_t default_value);
NU_API nu_result_t nu_config_get_string(const char *section, const char *name, const char **value, const char *default_value);
NU_API nu_result_t nu_config_get_bool(const char *section, const char *name, bool *value, bool default_value);

#endif