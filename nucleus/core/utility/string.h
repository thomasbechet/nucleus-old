#ifndef NU_STRING_H
#define NU_STRING_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>

NU_DECLARE_HANDLE(nu_string_t);
NU_DECLARE_HANDLE(nu_string_array_t);

NU_API nu_result_t nu_snprintf(char *str, size_t n, const char *format, ...);
NU_API nu_result_t nu_strtof(const char *str, float *v);
NU_API nu_result_t nu_strtod(const char *str, double *v);
NU_API nu_result_t nu_strtoi(const char *str, int32_t *v);
NU_API nu_result_t nu_strtou(const char *str, uint32_t *v);
NU_API nu_result_t nu_sscanf(const char *str, const char *format, ...);

NU_API void nu_string_allocate(nu_string_t *str);
NU_API void nu_string_allocate_capacity(nu_string_t *str, uint32_t capacity);
NU_API void nu_string_allocate_length(nu_string_t *str, uint32_t length);
NU_API void nu_string_allocate_cstr(nu_string_t *str, const char *cstr);
NU_API void nu_string_allocate_copy(nu_string_t *str, nu_string_t other);
NU_API void nu_string_allocate_substr_cstr(nu_string_t *str, const char *cstr, uint32_t index, uint32_t len);
NU_API void nu_string_allocate_substr(nu_string_t *str, nu_string_t other, uint32_t index, uint32_t len);
NU_API void nu_string_allocate_format(nu_string_t *str, const char *format, ...);
NU_API void nu_string_allocate_vformat(nu_string_t *str, const char *format, va_list args);
NU_API void nu_string_free(nu_string_t str);
NU_API void nu_string_clear(nu_string_t *str);
NU_API const char *nu_string_get_cstr(nu_string_t str);
NU_API char *nu_string_get_data(nu_string_t str);
NU_API uint32_t nu_string_get_length(nu_string_t str);
NU_API void nu_string_set_cstr(nu_string_t *str, const char *cstr);
NU_API void nu_string_set(nu_string_t *str, nu_string_t other);
NU_API void nu_string_append_cstr(nu_string_t *str, const char *cstr);
NU_API void nu_string_append(nu_string_t *str, nu_string_t other);
NU_API void nu_string_insert_cstr(nu_string_t *str, const char *cstr, uint32_t index);
NU_API void nu_string_insert(nu_string_t *str, nu_string_t other, uint32_t index);
NU_API uint32_t nu_string_find_first_cstr(nu_string_t str, const char *token);
NU_API uint32_t nu_string_find_first(nu_string_t str, nu_string_t token);
NU_API uint32_t nu_string_find_last_cstr(nu_string_t str, const char *token);
NU_API uint32_t nu_string_find_last(nu_string_t str, nu_string_t token);
NU_API uint32_t nu_string_replace_cstr(nu_string_t *str, const char *token, const char *other);
NU_API uint32_t nu_string_replace(nu_string_t *str, const char *token, nu_string_t other);
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