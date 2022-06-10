#ifndef NU_STRING_H
#define NU_STRING_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>
#include <nucleus/core/api/allocator.h>

#define NU_PATH_SEPARATOR        "/"
#define NU_PATH_ENGINE_DIRECTORY "engine/"
#define NU_PATH_MODULE_DIRECTORY "engine/module/"
#define NU_PATH_ROOT_DIRECTORY   ""

typedef char *nu_string_t;

NU_API nu_result_t nu_snprintf(char *str, uint32_t n, const char *format, ...);
NU_API nu_result_t nu_strtof(const char *str, float *v);
NU_API nu_result_t nu_strtod(const char *str, double *v);
NU_API nu_result_t nu_strtoi(const char *str, int32_t *v);
NU_API nu_result_t nu_strtou(const char *str, uint32_t *v);
NU_API uint32_t nu_strfirst(const char *str, uint32_t nstr, const char *token, uint32_t ntoken);
NU_API uint32_t nu_strlast(const char *str, uint32_t nstr, const char *token, uint32_t ntoken);
NU_API bool nu_strisdirectory(const char *path, uint32_t len);
NU_API bool nu_strisfilename(const char *path, uint32_t len);

NU_API nu_string_t nu_string_allocate_capacity_(nu_allocator_t allocator, uint32_t capacity, const char *file, uint32_t line);
NU_API nu_string_t nu_string_allocate_(nu_allocator_t allocator, const char *s, uint32_t len, const char *file, uint32_t line);
NU_API nu_string_t nu_string_allocate_format_(nu_allocator_t allocator, const char *file, uint32_t line, const char *format, ...);
NU_API nu_string_t nu_string_allocate_vformat_(nu_allocator_t allocator, const char *format, va_list args, const char *file, uint32_t line);
NU_API void nu_string_free_(nu_string_t str, const char *file, uint32_t line);
NU_API nu_string_t nu_string_clear(nu_string_t *str);
NU_API uint32_t nu_string_size(const nu_string_t str);
NU_API nu_string_t nu_string_append_(nu_string_t *pstr, const char *s, uint32_t len, const char *file, uint32_t line);
NU_API nu_string_t nu_string_append_format_(nu_string_t *str, const char *file, uint32_t line, const char *format, ...);
NU_API nu_string_t nu_string_append_vformat_(nu_string_t *str, const char *format, va_list args, const char *file, uint32_t line);
NU_API nu_string_t nu_string_insert_(nu_string_t *str, const char *s, uint32_t len, uint32_t index, const char *file, uint32_t line);
NU_API uint32_t nu_string_find_first_(const nu_string_t str, const char *token, uint32_t ntoken);
NU_API uint32_t nu_string_find_last_(const nu_string_t str, const char *token, uint32_t ntoken);
NU_API uint32_t nu_string_replace_(nu_string_t *str, const char *token, uint32_t ntoken, const char *other, uint32_t nother, const char *file, uint32_t line);
NU_API nu_string_t nu_string_erase(nu_string_t *str, uint32_t index, uint32_t len);
NU_API nu_string_t nu_string_trim(nu_string_t *str);
NU_API nu_string_t nu_string_set_(nu_string_t *str, const char *s, uint32_t len, const char *file, uint32_t line);

#define nu_string_allocate_empty(a) nu_string_allocate_capacity_(a, 0, __FILE__, __LINE__)
#define nu_string_allocate_capacity(a, capacity) nu_string_allocate_capacity_(a, capacity, __FILE__, __LINE__)
#define nu_string_allocate(a, s) nu_string_allocate_(a, s, (uint32_t)strlen(s), __FILE__, __LINE__)
#define nu_string_allocate_l(a, s, len) nu_string_allocate_(a, s, len, __FILE__, __LINE__)
#define nu_string_allocate_format(a, format, ...) nu_string_allocate_format_(a, __FILE__, __LINE__, format, __VA_ARGS__)
#define nu_string_allocate_vformat(a, format, args) nu_string_allocate_vformat_(a, format, args, __FILE__, __LINE__)
#define nu_string_free(str) nu_string_free_(str, __FILE__, __LINE__)
#define nu_string_append(pstr, s) nu_string_append_(pstr, s, (uint32_t)strlen(s), __FILE__, __LINE__)
#define nu_string_append_l(pstr, s, len) nu_string_append_(pstr, s, len, __FILE__, __LINE__)
#define nu_string_append_format(pstr, format, ...) nu_string_append_format_(pstr, __FILE__, __LINE__, format, __VA_ARGS__)
#define nu_string_append_vformat(pstr, format, args) nu_string_append_vformat_(pstr, format, args, __FILE__, __LINE__)
#define nu_string_insert(pstr, s, index) nu_string_insert_(pstr, s, (uint32_t)strlen(s), index, __FILE__, __LINE__)
#define nu_string_insert_l(pstr, s, len, index) nu_string_insert_(pstr, s, len, index, __FILE__, __LINE__)
#define nu_string_replace(pstr, token, cstr) nu_string_replace_(pstr, token, (uint32_t)strlen(token), cstr, strlen(cstr), __FILE__, __LINE__)
#define nu_string_set(pstr, s) nu_string_set_(pstr, s, (uint32_t)strlen(s), __FILE__, __LINE__)
#define nu_string_set_l(pstr, s, len) nu_string_set_(pstr, s, len, __FILE__, __LINE__)
#define nu_string_find_first(str, token) nu_string_find_first_(str, token, (uint32_t)strlen(token))
#define nu_string_find_last(str, token) nu_string_find_last_(str, token, (uint32_t)strlen(token))
#define nu_string_find_first_l(str, token, len) nu_string_find_first_(str, token, len)
#define nu_string_find_last_l(str, token, len) nu_string_find_last_(str, token, len)

NU_API nu_string_t nu_string_resolve_path_(nu_string_t *pstr, const char *file, uint32_t line);
NU_API nu_string_t nu_string_allocate_filename_(nu_allocator_t allocator, const char *path, const char *file, uint32_t line);
NU_API nu_string_t nu_string_allocate_directory_(nu_allocator_t allocator, const char *path, const char *file, uint32_t line);
NU_API nu_string_t nu_string_allocate_extension_(nu_allocator_t allocator, const char *path, const char *file, uint32_t line);

#define nu_string_resolve_path(pstr) (nu_string_resolve_path_(pstr, __FILE__, __LINE__))
#define nu_string_allocate_filename(a, path) (nu_string_allocate_filename_(a, path, __FILE__, __LINE__))
#define nu_string_allocate_directory(a, path) (nu_string_allocate_directory_(a, path, __FILE__, __LINE__))
#define nu_string_allocate_extension(a, path) (nu_string_allocate_extension_(a, path, __FILE__, __LINE__))
#define nu_string_is_directory(str) (nu_strisdirectory(str, nu_string_size(str)))
#define nu_string_is_filename(str) (nu_strisfilename_(str, nu_string_size(str)))

// NU_API nu_string_array_t nu_string_cstr_split(const char *cstr, const char *delim, nu_string_array_t tokens);
// NU_API void nu_string_split(nu_string_t str, const char *delim, nu_string_array_t tokens);

// NU_API nu_string_array_t nu_string_array_allocate(nu_allocator_t allocator);
// NU_API void nu_string_array_free(nu_string_array_t array);
// NU_API const char *nu_string_array_get(nu_string_array_t array, uint32_t index);
// NU_API uint32_t nu_string_array_get_length(nu_string_array_t array);
// NU_API void nu_string_array_add_ncstr(nu_string_array_t array, const char *cstr, uint32_t n);
// NU_API void nu_string_array_clear(nu_string_array_t array);

// NU_API void nu_string_resolve_path(nu_string_t *path);
// NU_API nu_string_t nu_string_allocate_path_filename(nu_allocator_t allocator, nu_string_t path);
// NU_API nu_string_t nu_string_allocate_path_directory(nu_allocator_t allocator, nu_string_t path);
// NU_API nu_string_t nu_string_allocate_cstr_directory(nu_allocator_t allocator, const char *path);
// NU_API nu_string_t nu_string_allocate_path_extension(nu_allocator_t allocator, nu_string_t path);
// NU_API bool nu_string_is_directory(nu_string_t path);
// NU_API bool nu_string_is_filename(nu_string_t path);

#endif