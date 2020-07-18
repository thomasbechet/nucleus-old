#ifndef NU_CONFIG_H
#define NU_CONFIG_H

#include "../system/window.h"
#include "../system/input.h"
#include "../system/renderer.h"

#define NU_CONFIG_CONTEXT_SECTION       "context"
#define NU_CONFIG_CONTEXT_VERSION_MAJOR "version_major"
#define NU_CONFIG_CONTEXT_VERSION_MINOR "version_minor"
#define NU_CONFIG_CONTEXT_VERSION_PATCH "version_patch"

#define NU_CONFIG_WINDOW_SECTION        "window"
#define NU_CONFIG_WINDOW_API            "api"

#define NU_CONFIG_INPUT_SECTION         "input"
#define NU_CONFIG_INPUT_API             "api"

#define NU_CONFIG_RENDERER_SECTION      "renderer"
#define NU_CONFIG_RENDERER_API          "api"

typedef struct {
    nu_window_api_t window_api;
    nu_input_api_t input_api;
    nu_renderer_api_t renderer_api;
} nu_config_t;

typedef nu_result_t (*nu_config_callback_t)(nu_config_t*);

nu_result_t nu_config_load(nu_config_callback_t callback);
nu_result_t nu_config_unload(void);

NU_API nu_config_t nu_config_get(void);

NU_API nu_result_t nu_config_get_int(const char *section, const char *name, int32_t *value, int32_t default_value);
NU_API nu_result_t nu_config_get_uint(const char *section, const char *name, uint32_t *value, uint32_t default_value);
NU_API nu_result_t nu_config_get_string(const char *section, const char *name, const char **value, const char *default_value);
NU_API nu_result_t nu_config_get_bool(const char *section, const char *name, bool *value, bool default_value);

#endif