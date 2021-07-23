#include <nucleus/core/utils/string.h>

#include <nucleus/core/coresystem/memory/memory.h>

typedef struct {
    uint32_t length;
    uint32_t capacity;
} nu_string_header_t;

static inline nu_string_header_t *nu_string_get_header_(nu_string_t str)
{
    return (nu_string_header_t*)str;
}
static inline char *nu_string_get_str_(nu_string_t str)
{
    return (char*)str + sizeof(nu_string_header_t);
}
static inline uint32_t nu_string_get_length_(nu_string_t str)
{
    return nu_string_get_header_(str)->length;
}
static inline uint32_t nu_string_get_capacity_(nu_string_t str)
{
    return nu_string_get_header_(str)->capacity;
}
static inline void nu_string_allocate_(uint32_t capacity, nu_string_t *str)
{
    *str = (nu_string_t)nu_malloc(sizeof(nu_string_header_t) + capacity + 1);
    nu_string_get_header_(*str)->capacity = capacity;
    nu_string_get_header_(*str)->length   = 0;
    nu_string_get_str_(*str)[0] = '\0';
}
static inline void nu_string_realloc_(nu_string_t *str, uint32_t capacity)
{
    *str = (nu_string_t)nu_realloc(*str, sizeof(nu_string_header_t) + capacity + 1);
    nu_string_get_header_(*str)->capacity = capacity;
}
static inline void nu_string_set_length_(nu_string_t *str, uint32_t len)
{
    nu_string_get_header_(*str)->length = len;
    nu_string_get_str_(*str)[len] = '\0';
}

void nu_string_allocate(nu_string_t *str)
{    
    nu_string_allocate_(0, str);
}
static inline void nu_string_allocate_ncstr(const char *cstr, uint32_t n, nu_string_t *str)
{
    nu_string_allocate_(n, str);
    memcpy(nu_string_get_str_(*str), cstr, n);
    nu_string_set_length_(str, n);
}
void nu_string_allocate_cstr(const char *cstr, nu_string_t *str)
{
    nu_string_allocate_ncstr(cstr, strlen(cstr), str);
}
void nu_string_allocate_copy(nu_string_t other, nu_string_t *str)
{
    nu_string_allocate_ncstr(nu_string_get_str_(other), nu_string_get_length_(other), str);
}
static inline void nu_string_allocate_substr_ncstr(const char *cstr, uint32_t n, uint32_t index, uint32_t len, nu_string_t *str)
{
    NU_ASSERT(index < n);
    NU_ASSERT(n > 0 && index + len <= n);
    nu_string_allocate_(len, str);
    memcpy(nu_string_get_str_(*str), cstr + index, len);
    nu_string_set_length_(str, len);
}
void nu_string_allocate_substr_cstr(const char *cstr, uint32_t index, uint32_t len, nu_string_t *str)
{
    nu_string_allocate_substr_ncstr(cstr, strlen(cstr), index, len, str);
}
void nu_string_allocate_substr(nu_string_t other, uint32_t index, uint32_t len, nu_string_t *str)
{
    nu_string_allocate_substr_ncstr(nu_string_get_str_(other), nu_string_get_length_(other),
        index, len, str);
}
void nu_string_allocate_format(nu_string_t *str, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    uint32_t n = vsnprintf(NULL, 0, format, args);
    va_end(args);
    nu_string_allocate_(n, str);
    va_start(args, format);
    vsnprintf(nu_string_get_str_(*str), n + 1, format, args);
    va_end(args);
    nu_string_set_length_(str, n);
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
static inline void nu_string_set_ncstr(nu_string_t *str, const char *cstr, uint32_t n)
{
    uint32_t capacity = nu_string_get_capacity_(*str);
    if (n > capacity) {
        nu_string_realloc_(str, n);
    }
    memcpy(nu_string_get_str_(*str), cstr, n);
    nu_string_set_length_(str, n);
}
void nu_string_set_cstr(nu_string_t *str, const char *cstr)
{
    nu_string_set_ncstr(str, cstr, strlen(cstr));
}
void nu_string_set(nu_string_t *str, nu_string_t other)
{
    NU_ASSERT(*str != other);
    nu_string_set_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other));
}
static inline void nu_string_append_ncstr(nu_string_t *str, const char *cstr, uint32_t n)
{
    if (n > 0) {
        uint32_t len = nu_string_get_length_(*str);
        if (len + n > nu_string_get_capacity_(*str)) {
            nu_string_realloc_(str, len + n);
        }
        memcpy(nu_string_get_str_(*str) + len, cstr, n);
        nu_string_set_length_(str, len + n);
    }
}
void nu_string_append_cstr(nu_string_t *str, const char *cstr)
{
    nu_string_append_ncstr(str, cstr, strlen(cstr));
}
void nu_string_append(nu_string_t *str, nu_string_t other)
{
    NU_ASSERT(*str != other);
    nu_string_append_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other));
}
static inline void nu_string_insert_ncstr(nu_string_t *str, const char *cstr, uint32_t n, uint32_t index)
{
    if (n > 0) {
        uint32_t len = nu_string_get_length_(*str);
        NU_ASSERT(index <= len);
        if (len + n > nu_string_get_capacity_(*str)) {
            nu_string_realloc_(str, len + n);
        }
        memmove(nu_string_get_str_(*str) + index + n, nu_string_get_str_(*str) + index, 
            len - index + 1);
        memcpy(nu_string_get_str_(*str) + index, cstr, n);
        nu_string_set_length_(str, len + n);
    }
}
void nu_string_insert_cstr(nu_string_t *str, const char *cstr, uint32_t index)
{
    nu_string_insert_ncstr(str, cstr, strlen(cstr), index);
}
void nu_string_insert(nu_string_t *str, nu_string_t other, uint32_t index)
{
    NU_ASSERT(*str != other);
    nu_string_insert_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other), index);
}
void nu_string_erase(nu_string_t *str, uint32_t index, uint32_t len)
{
    if (len > 0) {
        NU_ASSERT(index < nu_string_get_length_(*str));
        NU_ASSERT(index + len <= nu_string_get_length_(*str));
        memmove(nu_string_get_str_(*str) + index, nu_string_get_str_(*str) + index + len,
            nu_string_get_length_(*str) - len - index);
        nu_string_set_length_(str, nu_string_get_length_(*str) - len);
    }
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
        nu_string_realloc_(str, 0);
        return;
    }

    /* trim trailing space */
    char *pend = pstr + len - 1;
    while (pend > pstart && isspace(*pend)) pend--;

    uint32_t length = pend - pstart + 1;
    uint32_t index  = pstart - pstr;

    memmove(nu_string_get_str_(*str), nu_string_get_str_(*str) + index, length);
    nu_string_set_length_(str, length);
}
static inline void nu_string_split_ncstr(const char *cstr, uint32_t n, const char *delim, nu_string_array_t tokens)
{
    uint32_t start, i, len, dlen, tlen;
    const char *s;

    start = i = 0;
    len = n;
    s = cstr;
    dlen = strlen(delim);

    nu_string_array_clear(tokens);

    if (len == 0) return;

    for (i = 0; i < (len - dlen); i++) {
        /* search separator */
        if ((dlen == 1 && *(s + i) == delim[0]) || (memcmp(s + i, delim, dlen)) == 0) {
            tlen = i - start;
            if (tlen == 0 || (tlen == dlen && memcmp(s + start, delim, dlen) == 0)) {
                start = i + dlen;
                i += dlen - 1;
            } else {
                nu_string_array_add_ncstr(tokens, s + start, tlen);
                start = i + dlen;
                i += dlen - 1;
            }
        }
    }

    /* add final token */
    tlen = len - start;
    nu_string_array_add_ncstr(tokens, s + start, tlen);
}
void nu_string_split_cstr(const char *cstr, const char *delim, nu_string_array_t tokens)
{
    nu_string_split_ncstr(cstr, strlen(cstr), delim, tokens);
}
void nu_string_split(nu_string_t str, const char *delim, nu_string_array_t tokens)
{
    nu_string_split_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), delim, tokens);
}

typedef struct {
    uint32_t data_capacity;
    uint32_t index_capacity;
    uint32_t head;
    uint32_t length;
    char *data;
    uint32_t *indexes;
} nu_string_array_header_t;

#define NU_STRING_ARRAY_INITIAL_CAPACITY 20

void nu_string_array_allocate(nu_string_array_t *array)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)nu_malloc(sizeof(nu_string_array_header_t));
    header->data_capacity = NU_STRING_ARRAY_INITIAL_CAPACITY;
    header->index_capacity = NU_STRING_ARRAY_INITIAL_CAPACITY;
    header->head = 0;
    header->length = 0;
    header->data = (char*)nu_malloc(sizeof(char) * header->data_capacity);
    header->indexes = (uint32_t*)nu_malloc(sizeof(uint32_t) * header->index_capacity);
    *array = (nu_string_array_t)header;
}
void nu_string_array_free(nu_string_array_t array)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)array;
    nu_free(header->data);
    nu_free(header->indexes);
    nu_free(array);
}
const char *nu_string_array_get(nu_string_array_t array, uint32_t index)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)array;
    NU_ASSERT(index < header->length);
    return header->data + header->indexes[index];
}
uint32_t nu_string_array_get_length(nu_string_array_t array)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)array;
    return header->length;
}
void nu_string_array_add_ncstr(nu_string_array_t array, const char *cstr, uint32_t n)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)array;
    /* realloc data */
    if (header->head + n + 1 > header->data_capacity) {
        header->data_capacity *= 2;
        header->data = (char*)nu_realloc(header->data, header->data_capacity);
    }
    /* realloc indexes */
    if (header->length >= header->index_capacity) {
        header->index_capacity *= 2;
        header->indexes = (uint32_t*)nu_realloc(header->indexes, header->index_capacity);
    }
    /* save index */
    header->indexes[header->length] = header->head;
    /* copy the cstr */
    memcpy(header->data + header->head, cstr, n);
    header->data[header->head + n + 1] = '\0';
    /* udpate meta data */
    header->head += n + 2;
    header->length++;
}
void nu_string_array_clear(nu_string_array_t array)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)array;
    header->length = 0;
    header->head = 0;
}