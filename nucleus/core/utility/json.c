#include <nucleus/core/utility/json.h>

#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/coresystem/logger/interface.h>

#include <json/json.h>

static nu_json_type_t types_table[] = {
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
nu_json_value_t nu_json_get_root(nu_json_t json)
{
    return (nu_json_value_t)json;
}

nu_json_type_t nu_json_value_get_type(nu_json_value_t value)
{
    if (value == NU_NULL_HANDLE) return NU_JSON_TYPE_NULL;
    return types_table[((struct json_value_s*)value)->type];
}
nu_result_t nu_json_value_as_object(nu_json_value_t value, nu_json_object_t *object)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_OBJECT) return NU_FAILURE;
    *object = (nu_json_object_t)json_value_as_object((struct json_value_s*)value);
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_array(nu_json_value_t value, nu_json_array_t *array)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_ARRAY) return NU_FAILURE;
    *array = (nu_json_array_t)json_value_as_array((struct json_value_s*)value);
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_cstr(nu_json_value_t value, const char **cstr)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_STRING) return NU_FAILURE;
    *cstr = json_value_as_string((struct json_value_s*)value)->string;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_int(nu_json_value_t value, int32_t *i)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_NUMBER) return NU_FAILURE;
    return nu_strtoi(json_value_as_number(((struct json_value_s*)value))->number, i);
}
nu_result_t nu_json_value_as_uint(nu_json_value_t value, uint32_t *u)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_NUMBER) return NU_FAILURE;
    return nu_strtou(json_value_as_number(((struct json_value_s*)value))->number, u);
}
nu_result_t nu_json_value_as_bool(nu_json_value_t value, bool *b)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_BOOL) return NU_FAILURE;
    *b = (((struct json_value_s*)value)->type == json_type_true) ? true : false;
    return NU_SUCCESS; 
}
nu_result_t nu_json_value_as_float(nu_json_value_t value, float *f)
{
    if (value == NU_NULL_HANDLE || nu_json_value_get_type(value) != NU_JSON_TYPE_NUMBER) return NU_FAILURE;
    return nu_strtof(json_value_as_number(((struct json_value_s*)value))->number, f);
}

nu_json_value_t nu_json_object_get_by_name(nu_json_object_t object, const char *name)
{
    struct json_object_s *o = (struct json_object_s*)object;
    for (struct json_object_element_s *it = o->start; it != NULL; it = it->next) {
        if (strncmp(it->name->string, name, it->name->string_size) == 0) {
            return (nu_json_value_t)it->value;
        }
    }
    return NU_NULL_HANDLE;
}
uint32_t nu_json_object_get_length(nu_json_object_t object)
{
    struct json_object_s *o = (struct json_object_s*)object;
    return o->length;
}
bool nu_json_object_next(nu_json_object_t object, nu_json_object_iterator_t *it)
{
    if (*it == NU_NULL_HANDLE) {
        struct json_object_s *o = (struct json_object_s*)object;
        *it = (nu_json_object_iterator_t)o->start;
        return ((*it) != NULL);
    }
    struct json_object_element_s *e = (struct json_object_element_s*)(*it);
    if (e->next) {
        *it = (nu_json_object_iterator_t)e->next;
        return true;
    } else {
        *it = NU_NULL_HANDLE;
        return false;
    }    
}
const char *nu_json_object_iterator_get_name(nu_json_object_iterator_t it)
{
    return ((struct json_object_element_s*)it)->name->string;
}
nu_json_value_t nu_json_object_iterator_get_value(nu_json_object_iterator_t it)
{
    return (nu_json_value_t)(((struct json_object_element_s*)it)->value);
}

uint32_t nu_json_array_get_length(nu_json_array_t array)
{
    struct json_array_s *a = (struct json_array_s*)array;
    return a->length;
}
bool nu_json_array_next(nu_json_array_t array, nu_json_array_iterator_t *it)
{
    if (*it == NU_NULL_HANDLE) {
        struct json_array_s *a = (struct json_array_s*)array;
        *it = (nu_json_array_iterator_t)a->start;
        return ((*it) != NULL);
    }
    struct json_array_element_s *e = (struct json_array_element_s*)(*it);
    if (e->next) {
        *it = (nu_json_array_iterator_t)e->next;
        return true;
    } else {
        *it = NU_NULL_HANDLE;
        return false;
    }
}
nu_json_value_t nu_json_array_iterator_get_value(nu_json_array_iterator_t it)
{
    struct json_array_element_s *e = (struct json_array_element_s*)it;
    return (nu_json_value_t)e->value;
}