#include "config.h"

#include "../logger/logger.h"

#include <stdlib.h>
#include <ini/ini.h>

#define NU_CONFIG_LOG_NAME "[CONFIG] "
#define MAX_PARAMETER_COUNT 128

typedef struct {
    char *section;
    char *name;
    char *value;
} nu_config_parameter_t;

typedef struct {
    nu_config_t config;
    nu_config_parameter_t parameters[MAX_PARAMETER_COUNT];
    uint32_t parameter_count;
} nu_config_data_t;

static nu_config_data_t _data;

static void new_parameter(const char *section, const char *name, const char *value)
{
    nu_config_parameter_t *param = &_data.parameters[_data.parameter_count++];
    param->section = strdup(section);
    param->name    = strdup(name);
    param->value   = strdup(value);
}
static void delete_parameter(nu_config_parameter_t *param)
{
    free(param->section);
    free(param->name);
    free(param->value);
}

static int handler(void *user, const char *section, const char *name, const char *value)
{
    new_parameter(section, name, value);
    return 1;
}

static nu_result_t load_ini_file(void)
{
    _data.parameter_count = 0;

    if (ini_parse("engine/nucleus.ini", handler, NULL) < 0) {
        return NU_FAILURE;
    }

    /* context */
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_MAJOR, 0, &_data.config.context.version_major);
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_MINOR, 0, &_data.config.context.version_minor);
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_PATCH, 1, &_data.config.context.version_patch);
    nu_config_get_bool(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_LOG_CONFIG, true, &_data.config.context.log_config);

    /* window */
    const char *window_api;
    nu_config_get_string(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_API, NULL, &window_api);
    if (NU_MATCH(window_api, "glfw")) {
        _data.config.window.api = NU_WINDOW_API_GLFW;
    } else {
        _data.config.window.api = NU_WINDOW_API_NONE;
    }

    const char *window_mode;
    nu_config_get_string(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_MODE, "windowed", &window_mode);
    if (NU_MATCH(window_mode, "fullscreen")) {
        _data.config.window.mode = NU_WINDOW_MODE_FULLSCREEN;
    } else if (NU_MATCH(window_mode, "windowed")) {
        _data.config.window.mode = NU_WINDOW_MODE_WINDOWED;
    } else if (NU_MATCH(window_mode, "borderless")) {
        _data.config.window.mode = NU_WINDOW_MODE_BORDERLESS;
    }

    nu_config_get_uint(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_WIDTH, 1600, &_data.config.window.width);
    nu_config_get_uint(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_HEIGHT, 900, &_data.config.window.height);
    nu_config_get_bool(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_VSYNC, true, &_data.config.window.vsync);

    /* input */
    const char *input_api;
    nu_config_get_string(NU_CONFIG_INPUT_SECTION, NU_CONFIG_INPUT_API, "", &input_api);
    if (NU_MATCH(input_api, "glfw")) {
        _data.config.input.api = NU_INPUT_API_GLFW;
    } else {
        _data.config.input.api = NU_INPUT_API_NONE;
    }

    const char *input_cursor_mode;
    nu_config_get_string(NU_CONFIG_INPUT_SECTION, NU_CONFIG_INPUT_CURSOR_MODE, "normal", &input_cursor_mode);
    if (NU_MATCH(input_cursor_mode, "normal")) {
        _data.config.input.cursor_mode = NU_CURSOR_MODE_NORMAL;
    } else if (NU_MATCH(input_cursor_mode, "hidden")) {
        _data.config.input.cursor_mode = NU_CURSOR_MODE_HIDDEN;
    } else if (NU_MATCH(input_cursor_mode, "disable")) {
        _data.config.input.cursor_mode = NU_CURSOR_MODE_DISABLE;
    }

    /* renderer */
    const char *renderer_api;
    nu_config_get_string(NU_CONFIG_RENDERER_SECTION, NU_CONFIG_RENDERER_API, "", &renderer_api);
    if (NU_MATCH(renderer_api, "softrast")) {
        _data.config.renderer.api = NU_RENDERER_API_SOFTRAST;
    } else if (NU_MATCH(renderer_api, "raytracer")) {
        _data.config.renderer.api = NU_RENDERER_API_RAYTRACER;
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
            nu_config_unload();
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_unload(void)
{
    /* free memory from parameters */
    for (uint32_t i = 0; i < _data.parameter_count; i++) {
        delete_parameter(&_data.parameters[i]);
    }

    return NU_SUCCESS;
}

nu_config_t nu_config_get(void)
{
    return _data.config;
}
nu_result_t nu_config_get_string(const char *section, const char *name, const char *default_value, const char **value)
{
    const char *str = NULL;
    for (uint32_t i = 0; i < _data.parameter_count; i++) {
        if (NU_MATCH(section, _data.parameters[i].section) && NU_MATCH(name, _data.parameters[i].name)) {
            str = _data.parameters[i].value;
            break;
        }
    }
    *value = str ? str : default_value;

    return NU_SUCCESS;
}
nu_result_t nu_config_get_int(const char *section, const char *name, int32_t default_value, int32_t *value)
{
    const char *str;
    nu_config_get_string(section, name, NULL, &str);
    if (str) {
        char *t;
        *value = strtol(str, &t, 10);
        if (*t != '\0') return EXIT_FAILURE;
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_uint(const char *section, const char *name, uint32_t default_value, uint32_t *value)
{
    const char *str;
    nu_config_get_string(section, name, NULL, &str);
    if (str) {
        char *t;
        *value = strtoul(str, &t, 10);
        if (*t != '\0') return EXIT_FAILURE;
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_bool(const char *section, const char *name, bool default_value, bool *value)
{
    const char *str;
    nu_config_get_string(section, name, NULL, &str);
    if (str) {
        *value = (NU_MATCH(str, "true") || NU_MATCH(str, "True") || NU_MATCH(str, "1"));
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_float(const char *section, const char *name, float default_value, float *value)
{
    const char *str;
    nu_config_get_string(section, name, NULL, &str);
    if (str) {
        char *t;
        *value = strtof(str, &t);
        if (*t != '\0') return EXIT_FAILURE;
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}

static void log_transition_line(uint32_t max_section, uint32_t max_name, uint32_t max_value)
{
    nu_info("+");
    for (uint32_t i = 0; i < max_section + 2; i++) nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_name + 2; i++)    nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_value + 2; i++)   nu_info("-");
    nu_info("+\n");
}
static void log_line(
    uint32_t max_section, uint32_t max_name, uint32_t max_value,
    const char *section, const char *name, const char *value
)
{
    uint32_t section_len = 0;
    uint32_t name_len = strlen(name);
    uint32_t value_len = strlen(value);
    if (section) {
        section_len = strlen(section);
        nu_info("| %s", section);
    } else {
        nu_info("| ");
    }
    for (uint32_t i = 0; i < (max_section - section_len + 1); i++) nu_info(" ");
    nu_info("| %s", name);
    for (uint32_t i = 0; i < (max_name - name_len + 1); i++) nu_info(" ");
    nu_info("| %s", value);
    for (uint32_t i = 0; i < (max_value - value_len + 1); i++) nu_info(" ");
    nu_info("|\n");
}

nu_result_t nu_config_log(void)
{
    const char *section_header = "SECTION";
    const char *name_header    = "NAME";
    const char *value_header   = "VALUE";

    /* compute maximal length */
    uint32_t max_section = strlen(section_header);
    uint32_t max_name    = strlen(name_header);
    uint32_t max_value   = strlen(value_header);
    for (uint32_t i = 0; i < _data.parameter_count; i++) {
        max_section = NU_MAX(max_section, strlen(_data.parameters[i].section));
        max_name = NU_MAX(max_name, strlen(_data.parameters[i].name));
        max_value = NU_MAX(max_value, strlen(_data.parameters[i].value));
    }

    log_transition_line(max_section, max_name, max_value);
    log_line(max_section, max_name, max_value, section_header, name_header, value_header);

    char *current_section = "";
    for (uint32_t i = 0; i < _data.parameter_count; i++) {
        const nu_config_parameter_t *param = &_data.parameters[i];
        if (NU_MATCH(current_section, param->section)) {
            log_line(max_section, max_name, max_value, NULL, param->name, param->value);
        } else {
            log_transition_line(max_section, max_name, max_value);
            log_line(max_section, max_name, max_value, param->section, param->name, param->value);
            current_section = param->section;
        }
    }
    log_transition_line(max_section, max_name, max_value);

    return NU_SUCCESS;
}