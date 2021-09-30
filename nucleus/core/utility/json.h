#ifndef NU_JSON_H
#define NU_JSON_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>

NU_DECLARE_HANDLE(nu_json_t);
NU_DECLARE_HANDLE(nu_json_object_t);

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

NU_API bool nu_json_object_has(nu_json_object_t object, const char *key);
NU_API nu_result_t nu_json_object_get_string(nu_json_object_t object, const char *name, const char **value);

#endif