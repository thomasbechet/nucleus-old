#include <nucleus/core/config/config.h>

/* Dependencies with nu_core_log(), should be moved */
#include <nucleus/core/coresystem/logger/logger.h>

#include <stdlib.h>
#include <ini/ini.h>

#define MAX_PARAMETER_COUNT 128
#define MAX_SECTION_SIZE 50
#define MAX_NAME_SIZE 50
#define MAX_VALUE_SIZE 200

typedef struct {
    char section[MAX_SECTION_SIZE];
    char name[MAX_SECTION_SIZE];
    char value[MAX_VALUE_SIZE];
} nu_config_parameter_t;

typedef struct {
    nu_config_t config;
    nu_config_parameter_t parameters[MAX_PARAMETER_COUNT];
    uint32_t parameter_count;
} nu_system_data_t;

static nu_system_data_t _system;

static int handler(void *user, const char *section, const char *name, const char *value)
{
    (void)user;

    nu_config_parameter_t *param = &_system.parameters[_system.parameter_count++];
    strncpy(param->section, section, MAX_SECTION_SIZE);
    strncpy(param->name, name, MAX_NAME_SIZE);
    strncpy(param->value, value, MAX_VALUE_SIZE);

    return 1;
}

static nu_result_t load_ini_file(void)
{
    _system.parameter_count = 0;

    int code = ini_parse("engine/nucleus.ini", handler, NULL);
    NU_CHECK(code >= 0, return NU_FAILURE, NU_LOGGER_NAME, "Failed to parse ini file.");

    /* context */
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_MAJOR, 0, &_system.config.context.version_major);
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_MINOR, 0, &_system.config.context.version_minor);
    nu_config_get_uint(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_VERSION_PATCH, 1, &_system.config.context.version_patch);
    nu_config_get_bool(NU_CONFIG_CONTEXT_SECTION, NU_CONFIG_CONTEXT_LOG_CONFIG, true, &_system.config.context.log_config);

    /* logger */
    nu_config_get_bool(NU_CONFIG_LOGGER_SECTION, NU_CONFIG_LOGGER_ENABLE_LOG_FILE, false, &_system.config.logger.enable_log_file);
    nu_config_get_bool(NU_CONFIG_LOGGER_SECTION, NU_CONFIG_LOGGER_ENABLE_CORE_LOG_FILE, false, &_system.config.logger.enable_core_log_file);
    nu_config_get_cstr(NU_CONFIG_LOGGER_SECTION, NU_CONFIG_LOGGER_LOG_FILE_DIRECTORY, NU_PATH_ROOT_DIRECTORY, &_system.config.logger.log_file_directory);

    /* task */
    const char *task_api;
    nu_config_get_cstr(NU_CONFIG_TASK_SECTION, NU_CONFIG_TASK_API, NULL, &task_api);
    _system.config.task.api = NU_TASK_API_NONE;

    /* window */
    const char *window_api;
    nu_config_get_cstr(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_API, NULL, &window_api);
    if (NU_MATCH(window_api, "glfw")) {
        _system.config.window.api = NU_WINDOW_API_GLFW;
    } else {
        _system.config.window.api = NU_WINDOW_API_NONE;
    }

    const char *window_mode;
    nu_config_get_cstr(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_MODE, "windowed", &window_mode);
    if (NU_MATCH(window_mode, "fullscreen")) {
        _system.config.window.mode = NU_WINDOW_MODE_FULLSCREEN;
    } else if (NU_MATCH(window_mode, "windowed")) {
        _system.config.window.mode = NU_WINDOW_MODE_WINDOWED;
    } else if (NU_MATCH(window_mode, "borderless")) {
        _system.config.window.mode = NU_WINDOW_MODE_BORDERLESS;
    }

    nu_config_get_uint(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_WIDTH, 1600, &_system.config.window.width);
    nu_config_get_uint(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_HEIGHT, 900, &_system.config.window.height);
    nu_config_get_bool(NU_CONFIG_WINDOW_SECTION, NU_CONFIG_WINDOW_VSYNC, true, &_system.config.window.vsync);

    /* input */
    const char *input_api;
    nu_config_get_cstr(NU_CONFIG_INPUT_SECTION, NU_CONFIG_INPUT_API, "", &input_api);
    if (NU_MATCH(input_api, "glfw")) {
        _system.config.input.api = NU_INPUT_API_GLFW;
    } else {
        _system.config.input.api = NU_INPUT_API_NONE;
    }

    const char *input_cursor_mode;
    nu_config_get_cstr(NU_CONFIG_INPUT_SECTION, NU_CONFIG_INPUT_CURSOR_MODE, "normal", &input_cursor_mode);
    if (NU_MATCH(input_cursor_mode, "normal")) {
        _system.config.input.cursor_mode = NU_CURSOR_MODE_NORMAL;
    } else if (NU_MATCH(input_cursor_mode, "hidden")) {
        _system.config.input.cursor_mode = NU_CURSOR_MODE_HIDDEN;
    } else if (NU_MATCH(input_cursor_mode, "disable")) {
        _system.config.input.cursor_mode = NU_CURSOR_MODE_DISABLE;
    }

    /* renderer */
    const char *renderer_api;
    nu_config_get_cstr(NU_CONFIG_RENDERER_SECTION, NU_CONFIG_RENDERER_API, "", &renderer_api);
    if (NU_MATCH(renderer_api, "softrast")) {
        _system.config.renderer.api = NU_RENDERER_API_SOFTRAST;
    } else if (NU_MATCH(renderer_api, "vulkan")) {
        _system.config.renderer.api = NU_RENDERER_API_VULKAN;
    } else {
        _system.config.renderer.api = NU_RENDERER_API_NONE;
    }

    return NU_SUCCESS;
}

nu_result_t nu_config_load(nu_config_callback_pfn_t callback)
{
    nu_result_t result;

    memset(&_system.config, 0, sizeof(nu_config_t));

    result = load_ini_file();
    NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_NAME, "Failed to load configuration.");

    if (callback) {
        result = callback(&_system.config);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NU_LOGGER_NAME, "Error during user defined configuration.");
    }

    return NU_SUCCESS;

cleanup0:
    nu_config_unload();
    return NU_FAILURE;
}
nu_result_t nu_config_unload(void)
{
    return NU_SUCCESS;
}

nu_config_t nu_config_get(void)
{
    return _system.config;
}
nu_result_t nu_config_get_cstr(const char *section, const char *name, const char *default_value, const char **value)
{
    const char *str = NULL;
    for (uint32_t i = 0; i < _system.parameter_count; i++) {
        if (NU_MATCH(section, _system.parameters[i].section) && NU_MATCH(name, _system.parameters[i].name)) {
            str = _system.parameters[i].value;
            break;
        }
    }
    *value = str ? str : default_value;

    return NU_SUCCESS;
}
nu_result_t nu_config_get_int(const char *section, const char *name, int32_t default_value, int32_t *value)
{
    const char *str;
    nu_config_get_cstr(section, name, NULL, &str);
    if (str) {
        return nu_strtoi(str, value);
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_uint(const char *section, const char *name, uint32_t default_value, uint32_t *value)
{
    const char *str;
    nu_config_get_cstr(section, name, NULL, &str);
    if (str) {
        return nu_strtou(str, value);
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}
nu_result_t nu_config_get_bool(const char *section, const char *name, bool default_value, bool *value)
{
    const char *str;
    nu_config_get_cstr(section, name, NULL, &str);
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
    nu_config_get_cstr(section, name, NULL, &str);
    if (str) {
        return nu_strtof(str, value);
    } else {
        *value = default_value;
    }

    return NU_SUCCESS;
}

static void log_transition_line(uint32_t max_section, uint32_t max_name, uint32_t max_value)
{
    nu_string_t line;
    nu_string_allocate(&line);
    nu_string_append_cstr(&line, "+");
    for (uint32_t i = 0; i < max_section + 2; i++) nu_string_append_cstr(&line, "-");
    nu_string_append_cstr(&line, "+");
    for (uint32_t i = 0; i < max_name + 2; i++) nu_string_append_cstr(&line, "-");
    nu_string_append_cstr(&line, "+");
    for (uint32_t i = 0; i < max_value + 2; i++) nu_string_append_cstr(&line, "-");
    nu_string_append_cstr(&line, "+");

    nu_info(NU_LOGGER_NAME, "%s", nu_string_get_cstr(line));

    nu_string_free(line);
}
static void log_line(
    uint32_t max_section, uint32_t max_name, uint32_t max_value,
    const char *section, const char *name, const char *value
)
{
    nu_string_t line;
    nu_string_allocate(&line);

    uint32_t section_len = 0;
    uint32_t name_len = strlen(name);
    uint32_t value_len = strlen(value);
    if (section) {
        section_len = strlen(section);
        nu_string_append_cstr(&line, "| ");
        nu_string_append_cstr(&line, section);
    } else {
       nu_string_append_cstr(&line, "| ");
    }
    for (uint32_t i = 0; i < (max_section - section_len + 1); i++) nu_string_append_cstr(&line, " ");
    nu_string_append_cstr(&line, "| ");
    nu_string_append_cstr(&line, name);
    for (uint32_t i = 0; i < (max_name - name_len + 1); i++) nu_string_append_cstr(&line, " ");
    nu_string_append_cstr(&line, "| ");
    nu_string_append_cstr(&line, value);
    for (uint32_t i = 0; i < (max_value - value_len + 1); i++) nu_string_append_cstr(&line, " ");
    nu_string_append_cstr(&line, "|");

    nu_info(NU_LOGGER_NAME, nu_string_get_cstr(line));

    nu_string_free(line);
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
    for (uint32_t i = 0; i < _system.parameter_count; i++) {
        max_section = NU_MAX(max_section, strlen(_system.parameters[i].section));
        max_name = NU_MAX(max_name, strlen(_system.parameters[i].name));
        max_value = NU_MAX(max_value, strlen(_system.parameters[i].value));
    }

    log_transition_line(max_section, max_name, max_value);
    log_line(max_section, max_name, max_value, section_header, name_header, value_header);

    const char *current_section = "";
    for (uint32_t i = 0; i < _system.parameter_count; i++) {
        const nu_config_parameter_t *param = &_system.parameters[i];
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