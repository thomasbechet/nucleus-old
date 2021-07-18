#ifndef NU_STRING_H
#define NU_STRING_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_string_t);
NU_DECLARE_HANDLE(nu_string_tokens_t);

NU_API void nu_string_allocate_empty(nu_string_t *str);
NU_API void nu_string_allocate_from(const char *cstr, nu_string_t *str);
NU_API void nu_string_allocate_copy(nu_string_t other, nu_string_t *str);
NU_API void nu_string_allocate_substr(nu_string_t other, uint32_t index, uint32_t len, nu_string_t *str);
NU_API void nu_string_allocate_format(const char *format, nu_string_t *str, ...);
NU_API void nu_string_free(nu_string_t str);
NU_API const char *nu_string_get_cstr(nu_string_t str);
NU_API uint32_t nu_string_get_length(nu_string_t str);
NU_API void nu_string_append(nu_string_t *str, nu_string_t other);
NU_API void nu_string_insert(nu_string_t *str, nu_string_t other, uint32_t index);
NU_API void nu_string_erase(nu_string_t *str, uint32_t index, uint32_t len);
NU_API void nu_string_trim(nu_string_t *str);
NU_API void nu_string_split(nu_string_t str, const char *delim, nu_string_tokens_t *tokens);

NU_API const char *nu_string_tokens_get(nu_string_tokens_t tokens, uint32_t index);
NU_API uint32_t nu_string_tokens_get_length(nu_string_tokens_t tokens);
NU_API void nu_string_tokens_free(nu_string_tokens_t token);

#endif