#ifndef NU_STRING_H
#define NU_STRING_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_string_t);
NU_DECLARE_HANDLE(nu_string_array_t);

NU_API void nu_string_allocate(nu_string_t *str);
NU_API void nu_string_allocate_cstr(const char *cstr, nu_string_t *str);
NU_API void nu_string_allocate_copy(nu_string_t other, nu_string_t *str);
NU_API void nu_string_allocate_substr_cstr(const char *cstr, uint32_t index, uint32_t len, nu_string_t *str);
NU_API void nu_string_allocate_substr(nu_string_t other, uint32_t index, uint32_t len, nu_string_t *str);
NU_API void nu_string_allocate_format(nu_string_t *str, const char *format, ...);
NU_API void nu_string_free(nu_string_t str);
NU_API const char *nu_string_get_cstr(nu_string_t str);
NU_API uint32_t nu_string_get_length(nu_string_t str);
NU_API void nu_string_set_cstr(nu_string_t *str, const char *cstr);
NU_API void nu_string_set(nu_string_t *str, nu_string_t other);
NU_API void nu_string_append_cstr(nu_string_t *str, const char *cstr);
NU_API void nu_string_append(nu_string_t *str, nu_string_t other);
NU_API void nu_string_insert_cstr(nu_string_t *str, const char *cstr, uint32_t index);
NU_API void nu_string_insert(nu_string_t *str, nu_string_t other, uint32_t index);
NU_API void nu_string_erase(nu_string_t *str, uint32_t index, uint32_t len);
NU_API void nu_string_trim(nu_string_t *str);
NU_API void nu_string_split_cstr(const char *cstr, const char *delim, nu_string_array_t tokens);
NU_API void nu_string_split(nu_string_t str, const char *delim, nu_string_array_t tokens);

NU_API void nu_string_array_allocate(nu_string_array_t *array);
NU_API void nu_string_array_free(nu_string_array_t array);
NU_API const char *nu_string_array_get(nu_string_array_t array, uint32_t index);
NU_API uint32_t nu_string_array_get_length(nu_string_array_t array);
NU_API void nu_string_array_add_ncstr(nu_string_array_t array, const char *cstr, uint32_t n);
NU_API void nu_string_array_clear(nu_string_array_t array);

#endif