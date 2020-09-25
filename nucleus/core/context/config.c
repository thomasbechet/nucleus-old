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
    if (!_data.ini) return NU_FAILURE;

    /* context */
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_MAJOR, &_data.config.context.version_major, 0);
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_MINOR, &_data.config.context.version_minor, 0);
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_PATCH, &_data.config.context.version_patch, 1);
    nu_config_get_bool(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_LOG_CONFIG, &_data.config.context.log_config, true);

    /* window */
    const char *window_api;
    nu_config_get_string(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_API, &window_api, "");
    if (NU_MATCH(window_api, "glfw")) {
        _data.config.window.api = NU_WINDOW_API_GLFW;
    } else {
        _data.config.window.api = NU_WINDOW_API_NONE;
    }
    const char *window_mode;
    nu_config_get_string(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_MODE, &window_mode, "windowed");
    if (NU_MATCH(window_mode, "fullscreen")) {
        _data.config.window.mode = NU_WINDOW_MODE_FULLSCREEN;
    } else if (NU_MATCH(window_mode, "windowed")) {
        _data.config.window.mode = NU_WINDOW_MODE_WINDOWED;
    } else if (NU_MATCH(window_mode, "borderless")) {
        _data.config.window.mode = NU_WINDOW_MODE_BORDERLESS;
    }
    nu_config_get_uint(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_WIDTH, &_data.config.window.width, 1600);
    nu_config_get_uint(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_HEIGHT, &_data.config.window.height, 900);
    nu_config_get_bool(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_VSYNC, &_data.config.window.vsync, true);

    /* input */
    const char *input_api;
    nu_config_get_string(NU_CONFIG_INPUT_SECTION, NU_CONFIG_INPUT_API, &input_api, "");
    if (NU_MATCH(input_api, "glfw")) {
        _data.config.input.api = NU_INPUT_API_GLFW;
    } else {
        _data.config.input.api = NU_INPUT_API_NONE;
    }
    const char *input_cursor_mode;
    nu_config_get_string(NU_CONFIG_INPUT_SECTION, NU_CONFIG_INPUT_CURSOR_MODE, &input_cursor_mode, "normal");
    if (NU_MATCH(input_cursor_mode, "normal")) {
        _data.config.input.cursor_mode = NU_CURSOR_MODE_NORMAL;
    } else if (NU_MATCH(input_cursor_mode, "hidden")) {
        _data.config.input.cursor_mode = NU_CURSOR_MODE_HIDDEN;
    } else if (NU_MATCH(input_cursor_mode, "disable")) {
        _data.config.input.cursor_mode = NU_CURSOR_MODE_DISABLE;
    }

    /* renderer */
    const char *renderer_api;
    nu_config_get_string(NU_CONFIG_RENDERER_SECTION, NU_CONFIG_RENDERER_API, &renderer_api, "");
    if (NU_MATCH(renderer_api, "softrast")) {
        _data.config.renderer.api = NU_RENDERER_API_SOFTRAST;
    } else if (NU_MATCH(renderer_api, "vulkan")) {
        _data.config.renderer.api = NU_RENDERER_API_VULKAN;
    } else if (NU_MATCH(renderer_api, "opengl")) {
        _data.config.renderer.api = NU_RENDERER_API_OPENGL;
    } else {
        _data.config.renderer.api = NU_RENDERER_API_NONE;
    }

    return NU_SUCCESS;
}

nu_result_t nu_config_load(nu_config_callback_pfn_t callback)
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
nu_result_t nu_config_log(void)
{
    nu_info("===========================\n");
    nu_info("       CONFIGURATION       \n");
    nu_info("===========================\n");
    /* context */
    nu_info("[%s]\n", NU_CONFIG_CONTEXT_SECTION);
    nu_info("- version_major: %d\n", _data.config.context.version_major);
    nu_info("- version_minor: %d\n", _data.config.context.version_minor);
    nu_info("- version_patch: %d\n", _data.config.context.version_patch);
    /* window */
    nu_info("[%s]\n", NU_CONFIG_WINDOW_SECTION);
    switch (_data.config.window.api)
    {
    case NU_WINDOW_API_NONE: nu_info("- api: none\n"); break;
    case NU_WINDOW_API_GLFW: nu_info("- api: glfw\n"); break;
    }
    /* input */
    nu_info("[%s]\n", NU_CONFIG_INPUT_SECTION);
    switch (_data.config.input.api)
    {
    case NU_INPUT_API_NONE: nu_info("- api: none\n"); break;
    case NU_INPUT_API_GLFW: nu_info("- api: glfw\n"); break;
    }
    /* renderer */
    nu_info("[%s]\n", NU_CONFIG_RENDERER_SECTION);
    switch (_data.config.renderer.api)
    {
    case NU_RENDERER_API_NONE: nu_info("- api: none\n"); break;
    case NU_RENDERER_API_OPENGL: nu_info("- api: opengl\n"); break;
    case NU_RENDERER_API_SOFTRAST: nu_info("- api: software rasterizer\n"); break;
    case NU_RENDERER_API_VULKAN: nu_info("- api: vulkan\n"); break;
    }
    nu_info("===========================\n");

    return NU_SUCCESS;
}

nu_result_t nu_config_get_int(const char *section, const char *name, int32_t *value, int32_t default_value)
{
    const char *str = ini_get(_data.ini, section, name);
    if (str) {
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
    if (str) {
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
    if (str) {
        *value = (NU_MATCH(str, "true") || NU_MATCH(str, "True") || NU_MATCH(str, "1"));
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}