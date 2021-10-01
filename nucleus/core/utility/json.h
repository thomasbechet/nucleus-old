#ifndef NU_JSON_H
#define NU_JSON_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>

NU_DECLARE_HANDLE(nu_json_t);
NU_DECLARE_HANDLE(nu_json_value_t);
NU_DECLARE_HANDLE(nu_json_object_t);
NU_DECLARE_HANDLE(nu_json_object_iterator_t);
NU_DECLARE_HANDLE(nu_json_array_t);
NU_DECLARE_HANDLE(nu_json_array_iterator_t);

typedef enum {
    NU_JSON_TYPE_STRING,
    NU_JSON_TYPE_NUMBER,
    NU_JSON_TYPE_OBJECT,
    NU_JSON_TYPE_ARRAY,
    NU_JSON_TYPE_BOOL,
    NU_JSON_TYPE_NULL
} nu_json_type_t;

NU_API nu_result_t nu_json_allocate_from_file(nu_json_t *json, const char *filename);
NU_API nu_result_t nu_json_allocate_from_cstr(nu_json_t *json, const char *cstr);
NU_API void nu_json_free(nu_json_t json);
NU_API nu_json_value_t nu_json_get_root(nu_json_t json);

NU_API nu_json_type_t nu_json_value_get_type(nu_json_value_t value);
NU_API nu_result_t nu_json_value_as_object(nu_json_value_t value, nu_json_object_t *object);
NU_API nu_result_t nu_json_value_as_array(nu_json_value_t value, nu_json_array_t *array);
NU_API nu_result_t nu_json_value_as_cstr(nu_json_value_t value, const char **str);
NU_API nu_result_t nu_json_value_as_int(nu_json_value_t value, int32_t *i);
NU_API nu_result_t nu_json_value_as_uint(nu_json_value_t value, uint32_t *u);
NU_API nu_result_t nu_json_value_as_bool(nu_json_value_t value, bool *b);
NU_API nu_result_t nu_json_value_as_float(nu_json_value_t value, float *f);

NU_API nu_json_value_t nu_json_object_get_by_name(nu_json_object_t object, const char *name);
NU_API uint32_t nu_json_object_get_length(nu_json_object_t object);
NU_API bool nu_json_object_next(nu_json_object_t object, nu_json_object_iterator_t *it);
NU_API const char *nu_json_object_iterator_get_name(nu_json_object_iterator_t it);
NU_API nu_json_value_t nu_json_object_iterator_get_value(nu_json_object_iterator_t it);

NU_API uint32_t nu_json_array_get_length(nu_json_array_t array);
NU_API bool nu_json_array_next(nu_json_array_t array, nu_json_array_iterator_t *it);
NU_API nu_json_value_t nu_json_array_iterator_get_value(nu_json_array_iterator_t it);

#endif