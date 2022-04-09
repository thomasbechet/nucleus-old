#ifndef NU_JSON_H
#define NU_JSON_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>
#include <nucleus/core/utility/math/math.h>

#define NU_JSON_ITERATOR_INITIALIZE NU_NULL_HANDLE

NU_DECLARE_HANDLE(nu_json_t);
NU_DECLARE_HANDLE(nu_json_value_t);
NU_DECLARE_HANDLE(nu_json_object_t);
NU_DECLARE_HANDLE(nu_json_object_iterator_t);
NU_DECLARE_HANDLE(nu_json_array_t);
NU_DECLARE_HANDLE(nu_json_array_iterator_t);

NU_API nu_result_t nu_json_allocate_empty_object(nu_json_t *json);
NU_API nu_result_t nu_json_allocate_empty_array(nu_json_t *json);
NU_API nu_result_t nu_json_allocate_from_file(nu_json_t *json, const char *filename);
NU_API nu_result_t nu_json_allocate_from_cstr(nu_json_t *json, const char *cstr);
NU_API void nu_json_free(nu_json_t json);
NU_API nu_result_t nu_json_save_file(nu_json_t json, const char *filename, bool minify);
NU_API nu_json_value_t nu_json_get_root(nu_json_t json);

NU_API bool nu_json_value_is_string(nu_json_value_t value);
NU_API bool nu_json_value_is_number(nu_json_value_t value);
NU_API bool nu_json_value_is_object(nu_json_value_t value);
NU_API bool nu_json_value_is_array(nu_json_value_t value);
NU_API bool nu_json_value_is_bool(nu_json_value_t value);
NU_API bool nu_json_value_is_null(nu_json_value_t value);

NU_API nu_result_t nu_json_value_as_object(nu_json_value_t value, nu_json_object_t *object);
NU_API nu_result_t nu_json_value_as_array(nu_json_value_t value, nu_json_array_t *array);
NU_API nu_result_t nu_json_value_as_cstr(nu_json_value_t value, const char **str);
NU_API nu_result_t nu_json_value_as_int(nu_json_value_t value, int32_t *i);
NU_API nu_result_t nu_json_value_as_uint(nu_json_value_t value, uint32_t *u);
NU_API nu_result_t nu_json_value_as_bool(nu_json_value_t value, bool *b);
NU_API nu_result_t nu_json_value_as_float(nu_json_value_t value, float *f);
NU_API nu_result_t nu_json_value_as_double(nu_json_value_t value, double *d);
NU_API nu_result_t nu_json_value_as_vec3f(nu_json_value_t value, nu_vec3f_t v);
NU_API nu_result_t nu_json_value_as_quatf(nu_json_value_t value, nu_quatf_t q);
NU_API nu_result_t nu_json_value_as_transform(nu_json_value_t value, nu_transform_t *transform);

NU_API nu_json_value_t nu_json_object_get_by_name(nu_json_object_t object, const char *name);
NU_API bool nu_json_object_next(nu_json_object_t object, nu_json_object_iterator_t *it);
NU_API const char *nu_json_object_iterator_get_name(nu_json_object_iterator_t it);
NU_API nu_json_value_t nu_json_object_iterator_get_value(nu_json_object_iterator_t it);

NU_API uint32_t nu_json_array_get_size(nu_json_array_t array);
NU_API nu_json_value_t nu_json_array_get(nu_json_array_t array, uint32_t index);
NU_API bool nu_json_array_next(nu_json_array_t array, nu_json_array_iterator_t *it);
NU_API nu_json_value_t nu_json_array_iterator_get_value(nu_json_array_iterator_t it);

NU_API nu_result_t nu_json_object_put_empty_object(nu_json_object_t object, const char *name, nu_json_object_t *handle);
NU_API nu_result_t nu_json_object_put_empty_array(nu_json_object_t object, const char *name, nu_json_array_t *handle);
NU_API nu_result_t nu_json_object_put_null(nu_json_object_t object, const char *name);
NU_API nu_result_t nu_json_object_put_cstr(nu_json_object_t object, const char *name, const char *value);
NU_API nu_result_t nu_json_object_put_int(nu_json_object_t object, const char *name, int32_t i);
NU_API nu_result_t nu_json_object_put_uint(nu_json_object_t object, const char *name, uint32_t u);
NU_API nu_result_t nu_json_object_put_bool(nu_json_object_t object, const char *name, bool b);
NU_API nu_result_t nu_json_object_put_float(nu_json_object_t object, const char *name, float f);
NU_API nu_result_t nu_json_object_put_double(nu_json_object_t object, const char *name, double d);
NU_API nu_result_t nu_json_object_put_vec3f(nu_json_object_t object, const char *name, const nu_vec3f_t v);
NU_API nu_result_t nu_json_object_put_quatf(nu_json_object_t object, const char *name, const nu_quatf_t q);
NU_API nu_result_t nu_json_object_put_transform(nu_json_object_t object, const char *name, const nu_transform_t *transform);

NU_API nu_result_t nu_json_object_remove(nu_json_object_t object, const char *name);

NU_API nu_result_t nu_json_array_add_empty_object(nu_json_array_t array, nu_json_object_t *handle);
NU_API nu_result_t nu_json_array_add_empty_array(nu_json_array_t array, nu_json_array_t *handle);
NU_API nu_result_t nu_json_array_add_null(nu_json_array_t array);
NU_API nu_result_t nu_json_array_add_cstr(nu_json_array_t array, const char *value);
NU_API nu_result_t nu_json_array_add_int(nu_json_array_t array, int32_t value);
NU_API nu_result_t nu_json_array_add_uint(nu_json_array_t array, uint32_t value);
NU_API nu_result_t nu_json_array_add_bool(nu_json_array_t array, bool value);
NU_API nu_result_t nu_json_array_add_float(nu_json_array_t array, float value);
NU_API nu_result_t nu_json_array_add_double(nu_json_array_t array, double value);
NU_API nu_result_t nu_json_array_add_vec3f(nu_json_array_t array, const nu_vec3f_t v);
NU_API nu_result_t nu_json_array_add_transform(nu_json_array_t array, const nu_transform_t *transform);

NU_API nu_result_t nu_json_array_remove(nu_json_array_t array, uint32_t index);

#endif