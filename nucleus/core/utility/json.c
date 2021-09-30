#include <nucleus/core/utility/json.h>

#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/coresystem/logger/interface.h>

#include <json/json.h>

static nu_json_type_t type_table[] = {
    NU_JSON_TYPE_STRING,
    NU_JSON_TYPE_NUMBER,
    NU_JSON_TYPE_OBJECT,
    NU_JSON_TYPE_ARRAY,
    NU_JSON_TYPE_BOOL, /* true */
    NU_JSON_TYPE_BOOL, /* false */
    NU_JSON_TYPE_NULL
};

static void *nu_json_allocator(void *ud, size_t size)
{
    (void)ud;
    return nu_malloc(size);
}
static nu_result_t nu_json_parse(nu_json_t *json, const char *cstr, uint32_t len)
{
    struct json_parse_result_s parse_result;
    struct json_value_s *root = json_parse_ex(cstr, len, json_parse_flags_default, nu_json_allocator, NULL, &parse_result);
    NU_CHECK(root, return NU_FAILURE, NU_LOGGER_NAME, "Failed to parse json."); /* TODO: better error message */

    *json = (nu_json_t)root;

    return NU_SUCCESS;
}

nu_result_t nu_json_allocate_from_file(nu_json_t *json, const char *filename)
{
    nu_string_t path, json_str;
    nu_result_t result;
    
    nu_string_allocate_cstr(&path, filename);
    nu_string_resolve_path(&path);

    result = nu_io_readall_string(nu_string_get_cstr(path), &json_str);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NU_LOGGER_NAME, 
        "Failed to read json file: %s.", nu_string_get_cstr(path));

    result = nu_json_parse(json, nu_string_get_cstr(json_str), nu_string_get_length(json_str));

    nu_string_free(json_str);
cleanup0:
    nu_string_free(path);

    return result;
}
nu_result_t nu_json_allocate_from_cstr(nu_json_t *json, const char *cstr)
{
    return nu_json_parse(json, cstr, strlen(cstr));
}
void nu_json_free(nu_json_t json)
{
    nu_free((struct json_value_s*)json);
}

nu_json_type_t nu_json_get_type(nu_json_t json)
{
    struct json_value_s *v = (struct json_value_s*)json;
    return type_table[v->type];
}