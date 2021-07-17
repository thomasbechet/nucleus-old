#include <nucleus/core/utils/string.h>

#include <nucleus/core/memory/memory.h>
#include <nucleus/core/utils/array.h>

#define HEADER_LENGTH sizeof(uint32_t)

static inline uint32_t nu_string_get_total_allocation_(nu_string_t str)
{
    return HEADER_LENGTH + *(uint32_t*)str + 1;   
}
static inline uint32_t nu_string_get_length_(nu_string_t str)
{
    return *(uint32_t*)str;
}
static inline char *nu_string_get_str_(nu_string_t str)
{
    return (char*)((void*)str + HEADER_LENGTH);
}
static inline void nu_string_allocate_noninitialized_(nu_string_t *str, uint32_t len)
{
    *str = (nu_string_t)nu_malloc(HEADER_LENGTH + len + 1);
    *(uint32_t*)(*str) = len;
    nu_string_get_str_(*str)[len] = '\0';
}
static inline void nu_string_resize_(nu_string_t *str, uint32_t len)
{
    /* characters are copied */
    *str = (nu_string_t)nu_realloc(*str, HEADER_LENGTH + len + 1);
    *(uint32_t*)(*str) = len;
    nu_string_get_str_(*str)[len] = '\0';
}

void nu_string_allocate_empty(nu_string_t *str)
{    
    nu_string_allocate_noninitialized_(str, 0);
}
void nu_string_allocate_from(nu_string_t *str, const char *cstr)
{
    uint32_t len = strlen(cstr);
    nu_string_allocate_noninitialized_(str, len);
    memcpy(nu_string_get_str_(*str), cstr, len);
}
void nu_string_allocate_copy(nu_string_t *str, nu_string_t other)
{
    uint32_t allocation = nu_string_get_total_allocation_(other);
    *str = (nu_string_t)nu_malloc(allocation);
    memcpy(*str, other, allocation);
}
void nu_string_allocate_substr(nu_string_t *str, nu_string_t other, uint32_t index, uint32_t len)
{
    uint32_t otherlen = nu_string_get_length_(other);
    NU_ASSERT(index >= 0 && index < otherlen);
    NU_ASSERT(len > 0 && index + len <= otherlen);
    nu_string_allocate_noninitialized_(str, len);
    memcpy(nu_string_get_str_(*str), nu_string_get_str_(other) + index, len);
}
void nu_string_free(nu_string_t str)
{
    nu_free(str);
}
const char *nu_string_get_cstr(nu_string_t str)
{
    return nu_string_get_str_(str);
}
uint32_t nu_string_get_length(nu_string_t str)
{
    return nu_string_get_length_(str);
}
void nu_string_append(nu_string_t *str, nu_string_t other)
{
    NU_ASSERT(*str != other);
    uint32_t strlen   = nu_string_get_length_(*str);
    uint32_t otherlen = nu_string_get_length_(other);
    nu_string_resize_(str, strlen + otherlen);
    memcpy(nu_string_get_str_(*str) + strlen, nu_string_get_str_(other), otherlen);
}
void nu_string_insert(nu_string_t *str, nu_string_t other, uint32_t index)
{
    NU_ASSERT(*str != other);
    nu_string_t oldstr = *str;
    uint32_t strlen   = nu_string_get_length_(oldstr);
    uint32_t otherlen = nu_string_get_length_(other);
    NU_ASSERT(index >= 0 && index < strlen);
    nu_string_allocate_noninitialized_(str, strlen + otherlen);
    memcpy(nu_string_get_str_(*str), nu_string_get_str_(oldstr), index);
    memcpy(nu_string_get_str_(*str) + index, nu_string_get_str_(other), otherlen);
    memcpy(nu_string_get_str_(*str) + index + otherlen, nu_string_get_str_(oldstr) + index, strlen - index);
    nu_string_free(oldstr);
}
void nu_string_erase(nu_string_t *str, uint32_t index, uint32_t len)
{
    nu_string_t oldstr = *str;
    uint32_t strlen = nu_string_get_length_(oldstr);
    NU_ASSERT(index >= 0 && index < strlen);
    NU_ASSERT(len > 0 && index + len <= strlen);
    nu_string_allocate_noninitialized_(str, strlen - len);
    memcpy(nu_string_get_str_(*str), nu_string_get_str_(oldstr), index);
    memcpy(nu_string_get_str_(*str) + index, nu_string_get_str_(oldstr) + index + len, strlen - (index + len));
    nu_string_free(oldstr);
}
void nu_string_trim(nu_string_t *str)
{
    uint32_t len = nu_string_get_length_(*str);
    char *pstr = nu_string_get_str_(*str);
    if (len == 0) return;

    char *pstart = pstr;

    /* trim leading space */
    while (isspace(*pstart)) pstart++;

    /* all spaces ? */
    if (*pstart == '\0') {
        nu_string_resize_(str, 0);
        return;
    }

    /* trim trailing space */
    char *pend = pstr + len - 1;
    while (pend > pstart && isspace(*pend)) pend--;

    uint32_t length = pend - pstart + 1;
    uint32_t index  = pstart - pstr;

    nu_string_t nstr;
    nu_string_allocate_substr(&nstr, *str, index, length);
    nu_string_free(*str);
    *str = nstr;
}
void nu_string_split(nu_string_t str, const char *delim, nu_string_tokens_t *tokens)
{
    uint32_t start, i, len, dlen, tlen;
    nu_array_t list;
    const char *s;

    start = i = 0;
    len = nu_string_get_length_(str);
    s = nu_string_get_cstr(str);
    dlen = strlen(delim);

    nu_array_allocate_capacity(&list, sizeof(const char*), 5);

    if (len == 0) {
        *tokens = (nu_string_tokens_t)list;
        return;
    }

    for (i = 0; i < (len - dlen); i++) {
        /* search separator */
        if ((dlen == 1 && *(s + i) == delim[0]) || (memcmp(s + i, delim, dlen)) == 0) {
            tlen = i - start;
            if (tlen == 0 || (tlen == dlen && memcmp(s + start, delim, dlen) == 0)) {
                start = i + dlen;
                i += dlen - 1;
            } else {
                char *token = (char*)nu_malloc(sizeof(char) * (tlen + 1));
                token[tlen] = '\0';
                memcpy(token, s + start, tlen);
                nu_array_push(&list, &token);
                start = i + dlen;
                i += dlen - 1;
            }
        }
    }

    /* add final token */
    tlen = len - start;
    char *token = (char*)nu_malloc(sizeof(char) * (tlen + 1));
    token[tlen] = '\0';
    memcpy(token, s + start, tlen);
    nu_array_push(&list, &token);

    *tokens = (nu_string_tokens_t)list;
}

const char *nu_string_tokens_get(nu_string_tokens_t tokens, uint32_t index)
{
    return *(const char**)nu_array_get((nu_array_t)tokens, index);
}
uint32_t nu_string_tokens_get_length(nu_string_tokens_t tokens)
{
    return nu_array_get_size((nu_array_t)tokens);
}
void nu_string_tokens_free(nu_string_tokens_t token)
{
    uint32_t len = nu_array_get_size((nu_array_t)token);
    for (uint32_t i = 0; i < len; i++) {
        nu_free(*(char**)nu_array_get((nu_array_t)token, i));
    }
    nu_array_free((nu_array_t)token);
}