#include <nucleus/core/utility/json.h>

#include <nucleus/core/utility/io.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>

#include <cJSON/cJSON.h>

static nu_json_t nu_json_parse(const char *cstr, uint32_t len)
{
    cJSON *root = cJSON_ParseWithLength(cstr, len);
    NU_CHECK(root, return NU_FAILURE, nu_logger_get_core(), "Failed to parse json."); /* TODO: better error message */
    return (nu_json_t)root;
}

nu_json_t nu_json_allocate_empty_object(nu_allocator_t allocator)
{
    (void)allocator;
    return (nu_json_t)cJSON_CreateObject();
}
nu_json_t nu_json_allocate_empty_array(nu_allocator_t allocator)
{
    (void)allocator;
    return (nu_json_t)cJSON_CreateArray();
}
nu_json_t nu_json_allocate_from_file(nu_allocator_t allocator, const char *filename)
{
    nu_json_t json = NU_NULL_HANDLE;
        
    nu_string_t path = nu_string_allocate(allocator, filename);
    nu_string_resolve_path(&path);

    nu_string_t json_str = nu_io_readall_string(allocator, path);
    NU_CHECK(json_str != NU_NULL_HANDLE, goto cleanup0, nu_logger_get_core(), 
        "Failed to read json file: %s.", path);

    json = nu_json_parse(json_str, nu_string_size(json_str));

    nu_string_free(json_str);
cleanup0:
    nu_string_free(path);

    return json;
}
nu_json_t nu_json_allocate_from_cstr(nu_allocator_t allocator, const char *cstr)
{
    (void)allocator;
    return nu_json_parse(cstr, strlen(cstr));
}
nu_result_t nu_json_save_file(nu_json_t json, const char *filename, bool minify)
{
    nu_result_t result = NU_FAILURE;

    // Resolve path
    nu_string_t path = nu_string_allocate(nu_allocator_get_core(), filename);
    nu_string_resolve_path(&path);

    // Open file
    nu_file_t file = nu_file_open(path, NU_IO_MODE_WRITE);
    NU_CHECK(file != NU_NULL_HANDLE, goto cleanup0, nu_logger_get_core(),
        "Failed to open file: %s.", path);

    // Render json
    char *buffer = minify ? cJSON_PrintUnformatted((const cJSON*)json) : cJSON_Print((const cJSON*)json);
    NU_CHECK(file != NU_NULL_HANDLE, goto cleanup1, nu_logger_get_core(),
        "Failed to render json.");

    // Write file
    result = nu_file_write_cstr(file, buffer);
    NU_CHECK(result == NU_SUCCESS, goto cleanup2, nu_logger_get_core(),
        "Failed to write file.");

cleanup2:
    nu_free(nu_allocator_get_core(), buffer);
cleanup1:
    nu_file_close(file);
cleanup0:
    nu_string_free(path);

    return result;
}
void nu_json_free(nu_json_t json)
{
    cJSON_Delete((cJSON*)json);
}
nu_json_value_t nu_json_get_root(nu_json_t json)
{
    return (nu_json_value_t)json;
}

bool nu_json_value_is_string(nu_json_value_t value)
{
    return (value != NU_NULL_HANDLE && cJSON_IsString((const cJSON *const)value));
}
bool nu_json_value_is_number(nu_json_value_t value)
{
    return (value != NU_NULL_HANDLE && cJSON_IsNumber((const cJSON *const)value));
}
bool nu_json_value_is_object(nu_json_value_t value)
{
    return (value != NU_NULL_HANDLE && cJSON_IsObject((const cJSON *const)value));
}
bool nu_json_value_is_array(nu_json_value_t value)
{
    return (value != NU_NULL_HANDLE && cJSON_IsArray((const cJSON *const)value));
}
bool nu_json_value_is_bool(nu_json_value_t value)
{
    return (value != NU_NULL_HANDLE && cJSON_IsBool((const cJSON *const)value));
}
bool nu_json_value_is_null(nu_json_value_t value)
{
    return (value != NU_NULL_HANDLE && cJSON_IsNull((const cJSON *const)value));
}

nu_result_t nu_json_value_as_object(nu_json_value_t value, nu_json_object_t *object)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_object(value)) return NU_FAILURE;
    *object = (nu_json_object_t)value;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_array(nu_json_value_t value, nu_json_array_t *array)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_array(value)) return NU_FAILURE;
    *array = (nu_json_array_t)value;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_cstr(nu_json_value_t value, const char **cstr)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_string(value)) return NU_FAILURE;
    *cstr = ((cJSON*)value)->valuestring;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_int(nu_json_value_t value, int32_t *i)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_number(value)) return NU_FAILURE;
    *i = (int32_t)((cJSON*)value)->valuedouble;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_uint(nu_json_value_t value, uint32_t *u)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_number(value)) return NU_FAILURE;
    *u = (uint32_t)((cJSON*)value)->valuedouble;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_bool(nu_json_value_t value, bool *b)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_bool(value)) return NU_FAILURE;
    *b = cJSON_IsTrue((const cJSON *const)value) ? true : false;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_float(nu_json_value_t value, float *f)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_number(value)) return NU_FAILURE;
    *f = (float)((cJSON*)value)->valuedouble;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_double(nu_json_value_t value, double *d)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_number(value)) return NU_FAILURE;
    *d = (double)((cJSON*)value)->valuedouble;
    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_vec3f(nu_json_value_t value, nu_vec3f_t v)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_array(value)) return NU_FAILURE;
    
    nu_json_array_t array;
    nu_result_t result;
    
    result = nu_json_value_as_array(value, &array);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Json value is not an array.");
    
    nu_vec3f_zero(v);

    nu_json_array_iterator_t it = NU_NULL_HANDLE;
    uint32_t index = 0;
    while (nu_json_array_next(array, &it) && index <= 2) {
        result = nu_json_value_as_float(nu_json_array_iterator_get_value(it), &v[index]);
        NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Json vec3f %d component is not a float.", index);
        index++;
    }

    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_quatf(nu_json_value_t value, nu_quatf_t q)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_array(value)) return NU_FAILURE;
    
    nu_json_array_t array;
    nu_result_t result;
    
    result = nu_json_value_as_array(value, &array);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Json value is not an array.");
    
    nu_quatf_identity(q);

    nu_json_array_iterator_t it = NU_NULL_HANDLE;
    uint32_t index = 0;
    while (nu_json_array_next(array, &it) && index <= 3) {
        result = nu_json_value_as_float(nu_json_array_iterator_get_value(it), &q[index]);
        NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Json quatf %d component is not a float.", index);
        index++;
    }

    return NU_SUCCESS;
}
nu_result_t nu_json_value_as_transform(nu_json_value_t value, nu_transform_t *transform)
{
    if (value == NU_NULL_HANDLE || !nu_json_value_is_object(value)) return NU_FAILURE;

    nu_result_t result;
    nu_json_object_t object;
    result = nu_json_value_as_object(value, &object);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Failed to parse transform object.");

    nu_json_value_t j_translation = nu_json_object_get_by_name(object, "translation");
    if (j_translation != NU_NULL_HANDLE) {
        result = nu_json_value_as_vec3f(j_translation, transform->translation);
        NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Failed to parse translation component for transform.");
    }

    nu_json_value_t j_scale = nu_json_object_get_by_name(object, "scale");
    if (j_scale != NU_NULL_HANDLE) {
        result = nu_json_value_as_vec3f(j_scale, transform->scale);
        NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Failed to parse scale component for transform.");
    }

    nu_json_value_t j_rotation = nu_json_object_get_by_name(object, "rotation");
    if (j_rotation != NU_NULL_HANDLE) {
        result = nu_json_value_as_quatf(j_rotation, transform->rotation);
        NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, nu_logger_get_core(), "Failed to parse rotation component for transform.");
    }

    return NU_SUCCESS;
}

nu_json_value_t nu_json_object_get_by_name(nu_json_object_t object, const char *name)
{
    const cJSON *value = cJSON_GetObjectItemCaseSensitive((const cJSON *const)object, name);
    return value ? (nu_json_value_t)value : NU_NULL_HANDLE;
}
bool nu_json_object_next(nu_json_object_t object, nu_json_object_iterator_t *it)
{
    if (*it == NU_NULL_HANDLE) {
        cJSON *o = (cJSON*)object;
        *it = (nu_json_object_iterator_t)o->child;
        return ((*it) != NULL);
    }
    cJSON *e = (cJSON*)(*it);
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
    return ((cJSON*)it)->string;
}
nu_json_value_t nu_json_object_iterator_get_value(nu_json_object_iterator_t it)
{
    return (nu_json_value_t)it;
}

uint32_t nu_json_array_get_size(nu_json_array_t array)
{
    return (uint32_t)cJSON_GetArraySize((cJSON*)array);
}
nu_json_value_t nu_json_array_get(nu_json_array_t array, uint32_t index)
{
    cJSON *i = cJSON_GetArrayItem((const cJSON*)array, (int)index);
    return i ? (nu_json_value_t)i : NU_NULL_HANDLE;
}
bool nu_json_array_next(nu_json_array_t array, nu_json_array_iterator_t *it)
{
    if (*it == NU_NULL_HANDLE) {
        cJSON *a = (cJSON*)array;
        *it = (nu_json_array_iterator_t)a->child;
        return ((*it) != NULL);
    }
    cJSON *e = (cJSON*)(*it);
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
    return (nu_json_value_t)it;
}

nu_result_t nu_json_object_put_empty_object(nu_json_object_t object, const char *name, nu_json_object_t *handle)
{
    return ((*handle = (nu_json_object_t)cJSON_AddObjectToObject((cJSON *const)object, name)) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_empty_array(nu_json_object_t object, const char *name, nu_json_array_t *handle)
{
    return ((*handle = (nu_json_array_t)cJSON_AddArrayToObject((cJSON *const)object, name)) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_null(nu_json_object_t object, const char *name)
{
    return (cJSON_AddNullToObject((cJSON *const)object, name) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_cstr(nu_json_object_t object, const char *name, const char *value)
{
    return (cJSON_AddStringToObject((cJSON *const)object, name, value) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_int(nu_json_object_t object, const char *name, int32_t i)
{
    return (cJSON_AddNumberToObject((cJSON *const)object, name, (const double)i) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_uint(nu_json_object_t object, const char *name, uint32_t u)
{
    return (cJSON_AddNumberToObject((cJSON *const)object, name, (const double)u) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_bool(nu_json_object_t object, const char *name, bool b)
{
    return (cJSON_AddBoolToObject((cJSON *const)object, name, b) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_float(nu_json_object_t object, const char *name, float f)
{
    return (cJSON_AddNumberToObject((cJSON *const)object, name, (const double)f) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_double(nu_json_object_t object, const char *name, double d)
{
    return (cJSON_AddNumberToObject((cJSON *const)object, name, (const double)d) != NULL) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_vec3f(nu_json_object_t object, const char *name, const nu_vec3f_t v)
{
    cJSON *a = cJSON_CreateFloatArray(v, 3);
    return cJSON_AddItemToObject((cJSON*)object, name, a) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_quatf(nu_json_object_t object, const char *name, const nu_quatf_t q)
{
    cJSON *a = cJSON_CreateFloatArray(q, 4);
    return cJSON_AddItemToObject((cJSON*)object, name, a) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_object_put_transform(nu_json_object_t object, const char *name, const nu_transform_t *transform)
{
    nu_json_object_t handle;
    nu_json_object_put_empty_object(object, name, &handle);
    nu_json_object_put_vec3f(handle, "translation", transform->translation);
    nu_json_object_put_vec3f(handle, "scale", transform->scale);
    nu_json_object_put_quatf(handle, "rotation", transform->rotation);
    return NU_SUCCESS;
}

nu_result_t nu_json_object_remove(nu_json_object_t object, const char *name)
{
    cJSON_DeleteItemFromObject((cJSON*)object, name);
    return NU_SUCCESS;
}

nu_result_t nu_json_array_add_empty_object(nu_json_array_t array, nu_json_object_t *handle)
{
    cJSON *i = cJSON_CreateObject();
    *handle = (nu_json_object_t)i;
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_empty_array(nu_json_array_t array, nu_json_array_t *handle)
{
    cJSON *i = cJSON_CreateArray();
    *handle = (nu_json_array_t)i;
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_null(nu_json_array_t array)
{
    cJSON *i = cJSON_CreateNull();
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_cstr(nu_json_array_t array, const char *value)
{
    cJSON *i = cJSON_CreateString(value);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_int(nu_json_array_t array, int32_t value)
{
    cJSON *i = cJSON_CreateNumber((double)value);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_uint(nu_json_array_t array, uint32_t value)
{
    cJSON *i = cJSON_CreateNumber((double)value);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_bool(nu_json_array_t array, bool value)
{
    cJSON *i = cJSON_CreateBool(value);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_float(nu_json_array_t array, float value)
{
    cJSON *i = cJSON_CreateNumber((double)value);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_double(nu_json_array_t array, double value)
{
    cJSON *i = cJSON_CreateNumber((double)value);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_vec3f(nu_json_array_t array, const nu_vec3f_t v)
{
    cJSON *i = cJSON_CreateFloatArray(v, 3);
    return cJSON_AddItemToArray((cJSON*)array, i) ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_json_array_add_transform(nu_json_array_t array, const nu_transform_t *transform)
{
    nu_json_object_t handle;
    nu_json_array_add_empty_object(array, &handle);
    nu_json_object_put_vec3f(handle, "translation", transform->translation);
    nu_json_object_put_vec3f(handle, "scale", transform->scale);
    nu_json_object_put_quatf(handle, "rotation", transform->rotation);
    return NU_SUCCESS;
}

nu_result_t nu_json_array_remove(nu_json_array_t array, uint32_t index)
{
    nu_result_t result = index < nu_json_array_get_size(array) ? NU_SUCCESS : NU_FAILURE;
    cJSON_DeleteItemFromArray((cJSON*)array, index);
    return result;
}