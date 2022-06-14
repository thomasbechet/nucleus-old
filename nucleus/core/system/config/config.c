#include <nucleus/core/system/config/config.h>

#include <nucleus/core/system/config/api.h>
#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/system/module/module.h>
#include <nucleus/core/system/module/api.h>
#include <nucleus/core/utility/table_printer.h>

#include <ini/ini.h>

#define MAX_SECTION_SIZE    32
#define MAX_NAME_SIZE       32
#define MAX_VALUE_SIZE      128

typedef struct {
    char section[MAX_SECTION_SIZE];
    char name[MAX_SECTION_SIZE];
    char value[MAX_VALUE_SIZE];
} nu_config_parameter_t;

typedef struct {
    nu_vector(nu_config_parameter_t) parameters;
} nu_state_t;

static nu_state_t s_state;

static const nu_config_api_t s_config_api = {
    .load      = nu_config_load,
    .get_cstr  = nu_config_get_cstr,
    .get_int   = nu_config_get_int,
    .get_uint  = nu_config_get_uint,
    .get_bool  = nu_config_get_bool,
    .get_float = nu_config_get_float,
    .log       = nu_config_log
};

static int handler(void *user, const char *section, const char *name, const char *value)
{
    (void)user;

    // Store data
    nu_config_parameter_t *param = nu_vector_push(&s_state.parameters);
    strncpy(param->section, section, MAX_SECTION_SIZE - 1);
    strncpy(param->name, name, MAX_NAME_SIZE - 1);
    strncpy(param->value, value, MAX_VALUE_SIZE - 1);

    return 1;
}

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_config_initialize(void)
{
    s_state.parameters = nu_vector_allocate_capacity(nu_allocator_get_core(), nu_config_parameter_t, 128);
    return NU_SUCCESS;
}
nu_result_t nu_config_terminate(void)
{
    nu_vector_free(s_state.parameters);
    return NU_SUCCESS;
}
nu_result_t nu_config_configure(bool load_engine_config)
{
    if (load_engine_config) {
        return nu_config_load("$engine/engine.ini");
    }
    return NU_SUCCESS;
}
nu_result_t nu_config_register_api(void)
{
    return nu_module_register_api(nu_module_get_core(), nu_config_api_t, &s_config_api);
}

// +--------------------------------------------------------------------------+
// |                               PUBLIC API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_config_load(const char *path)
{
    nu_string_t resolved_path = nu_string_allocate(nu_allocator_get_core(), path);
    nu_string_resolve_path(&resolved_path);
    int code = ini_parse(resolved_path, handler, NULL);
    nu_string_free(resolved_path);
    NU_CHECK(code >= 0, return NU_FAILURE, nu_logger_get_core(), "Failed to load configuration file: '%s'.", path);
    return NU_SUCCESS;
}
const char *nu_config_get_cstr(const char *section, const char *name, const char *default_value)
{
    const char *str = NULL;
    for (nu_config_parameter_t *it = s_state.parameters; it != nu_vector_end(s_state.parameters); it++) {
        if (NU_MATCH(section, it->section) && NU_MATCH(name, it->name)) {
            str = it->value;
            break;
        }
    }
    return str ? str : default_value;
}
int32_t nu_config_get_int(const char *section, const char *name, int32_t default_value)
{
    const char *str = nu_config_get_cstr(section, name, NULL);
    if (str) {
        int32_t val; 
        return nu_strtoi(str, &val) == NU_SUCCESS ? val : default_value;
    } else {
        return default_value;
    }
}
uint32_t nu_config_get_uint(const char *section, const char *name, uint32_t default_value)
{
    const char *str = nu_config_get_cstr(section, name, NULL);
    if (str) {
        uint32_t val; 
        return nu_strtou(str, &val) == NU_SUCCESS ? val : default_value;
    } else {
        return default_value;
    }
}
bool nu_config_get_bool(const char *section, const char *name, bool default_value)
{
    const char *str = nu_config_get_cstr(section, name, NULL);
    if (str) {
        return (NU_MATCH(str, "true") || NU_MATCH(str, "True") || NU_MATCH(str, "1"));
    } else {
        return default_value;
    }
}
float nu_config_get_float(const char *section, const char *name, float default_value)
{
    const char *str = nu_config_get_cstr(section, name, NULL);
    if (str) {
        float val;
        return nu_strtof(str, &val) == NU_SUCCESS ? val : default_value;
    } else {
        return default_value;
    }
}
void nu_config_log(void)
{
    nu_table_printer_t tp = nu_table_printer_allocate(nu_allocator_get_core());
    
    // Header
    nu_table_printer_row_center(tp, "%s", "CONFIGURATIONS");
    nu_table_printer_row_center(tp, "%s|%s|%s", "SECTION", "NAME", "VALUE");
    nu_table_printer_separator(tp);
    
    // Log configuration
    for (nu_config_parameter_t *it = s_state.parameters; it != nu_vector_end(s_state.parameters); it++) {
        nu_table_printer_row(tp, "%s|%s|%s", it->section, it->name, it->value);
    }

    // Render and free resources
    nu_info(nu_logger_get_core(), "Configurations information :\n%s", nu_table_printer_render(tp));
    nu_table_printer_free(tp);
}