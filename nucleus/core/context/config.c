#include "config.h"

#include <stdlib.h>

#include <ini/ini.h>

#define NU_CONFIG_LOG_NAME "[CONFIG] "

typedef struct {
    ini_t *ini;
    nu_config_t config;
} config_data_t;

static config_data_t _data;

static nu_result_t load_ini_file(void)
{
    _data.ini = ini_load("engine/nucleus.ini");
    if (!_data.ini) {
        nu_warning(NU_CONFIG_LOG_NAME"Failed to load ini file.\n");
        return NU_FAILURE;
    }

    /* window api */
    const char *window_api;
    nu_config_get_string(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_API, &window_api, "");
    if (NU_MATCH(window_api, "glfw")) {
        _data.config.window_api = NU_WINDOW_API_GLFW;
    } else {
        _data.config.window_api = NU_WINDOW_API_NONE;
    }

    /* renderer api */
    const char *renderer_api;
    nu_config_get_string(NU_CONFIG_RENDERER_SECTION, NU_CONFIG_RENDERER_API, &renderer_api, "");
    if (NU_MATCH(renderer_api, "softrast")) {
        _data.config.renderer_api = NU_RENDERER_API_SOFTRAST;
    } else if (NU_MATCH(renderer_api, "vulkan")) {
        _data.config.renderer_api = NU_RENDERER_API_VULKAN;
    } else if (NU_MATCH(renderer_api, "opengl")) {
        _data.config.renderer_api = NU_RENDERER_API_OPENGL;
    } else {
        _data.config.renderer_api = NU_RENDERER_API_NONE;
    }

    return NU_SUCCESS;
}

nu_result_t nu_config_load(nu_config_callback_t callback)
{
    memset(&_data.config, 0, sizeof(nu_config_t));

    if (load_ini_file() != NU_SUCCESS) {
        nu_warning(NU_CONFIG_LOG_NAME"Failed to load ini file.\n");
        return NU_FAILURE;
    }

    if (callback) {
        if (callback(&_data.config) != NU_SUCCESS) {
            nu_warning(NU_CONFIG_LOG_NAME"Error during configuration check.\n");
            ini_free(_data.ini);
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_unload(void)
{
    if (_data.ini) {
        ini_free(_data.ini);
    }

    return NU_SUCCESS;
}

nu_config_t nu_config_get(void)
{
    return _data.config;
}

nu_result_t nu_config_get_int(const char *section, const char *name, int32_t *value, int32_t default_value)
{
    const char *str = ini_get(_data.ini, section, name);
    if (!str) {
        char *t;
        *value = strtol(str, &t, 10);
        if (*t != '\0') return EXIT_FAILURE;
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_uint(const char *section, const char *name, uint32_t *value, uint32_t default_value)
{
    const char *str = ini_get(_data.ini, section, name);
    if (!str) {
        char *t;
        *value = strtoul(str, &t, 10);
        if (*t != '\0') return EXIT_FAILURE;
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_string(const char *section, const char *name, const char **value, const char *default_value)
{
    const char *str = ini_get(_data.ini, section, name);
    *value = str ? str : default_value;

    return NU_SUCCESS;
}
nu_result_t nu_config_get_bool(const char *section, const char *name, bool *value, bool default_value)
{
    const char *str = ini_get(_data.ini, section, name);
    if (!str) {
        *value = (NU_MATCH(str, "true") || NU_MATCH(str, "True") || NU_MATCH(str, "1"));
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}