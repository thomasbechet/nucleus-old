#include <nucleus/core/utility/string.h>

#include <nucleus/core/coresystem/memory/memory.h>

nu_result_t nu_snprintf(char *str, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int32_t code = vsnprintf(str, n, format, args);
    va_end(args);
    return code >= 0 ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_strtof(const char *str, float *v)
{
    char *t;
    *v = strtof(str, &t);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}
nu_result_t nu_strtod(const char *str, double *v)
{
    char *t;
    *v = strtod(str, &t);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}
nu_result_t nu_strtoi(const char *str, int32_t *v)
{
    char *t;
    *v = strtol(str, &t, 10);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}
nu_result_t nu_strtou(const char *str, uint32_t *v)
{
    char *t;
    *v = strtoul(str, &t, 10);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}

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
static inline void nu_string_allocate_(nu_string_t *str, uint32_t capacity)
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
    nu_string_allocate_(str, 0);
}
void nu_string_allocate_capacity(nu_string_t *str, uint32_t capacity)
{
    nu_string_allocate_(str, capacity);
}
void nu_string_allocate_length(nu_string_t *str, uint32_t length)
{
    nu_string_allocate_(str, length);
    nu_string_set_length_(str, length);
}
static inline void nu_string_allocate_ncstr(nu_string_t *str, const char *cstr, uint32_t n)
{
    nu_string_allocate_(str, n);
    memcpy(nu_string_get_str_(*str), cstr, n);
    nu_string_set_length_(str, n);
}
void nu_string_allocate_cstr(nu_string_t *str, const char *cstr)
{
    nu_string_allocate_ncstr(str, cstr, strlen(cstr));
}
void nu_string_allocate_copy(nu_string_t *str, nu_string_t other)
{
    nu_string_allocate_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other));
}
static inline void nu_string_allocate_substr_ncstr(nu_string_t *str, const char *cstr, uint32_t n, uint32_t index, uint32_t len)
{
    NU_ASSERT(index < n);
    NU_ASSERT(n > 0 && index + len <= n);
    len = NU_MIN(index + len, n);
    nu_string_allocate_(str, len);
    memcpy(nu_string_get_str_(*str), cstr + index, len);
    nu_string_set_length_(str, len);
}
void nu_string_allocate_substr_cstr(nu_string_t *str, const char *cstr, uint32_t index, uint32_t len)
{
    nu_string_allocate_substr_ncstr(str, cstr, strlen(cstr), index, len);
}
void nu_string_allocate_substr(nu_string_t *str, nu_string_t other, uint32_t index, uint32_t len)
{
    nu_string_allocate_substr_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other), index, len);
}
void nu_string_allocate_format(nu_string_t *str, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_string_allocate_vformat(str, format, args);
    va_end(args);
}
void nu_string_allocate_vformat(nu_string_t *str, const char *format, va_list args)
{
    va_list args_temp;
    va_copy(args_temp, args);
    uint32_t n = vsnprintf(NULL, 0, format, args);
    va_end(args_temp);
    nu_string_allocate_(str, n);
    va_copy(args_temp, args);
    vsnprintf(nu_string_get_str_(*str), n + 1, format, args);
    va_end(args_temp);
    nu_string_set_length_(str, n);
}
void nu_string_free(nu_string_t str)
{
    nu_free(str);
}
void nu_string_clear(nu_string_t *str)
{
    nu_string_set_length_(str, 0);
}
const char *nu_string_get_cstr(nu_string_t str)
{
    return nu_string_get_str_(str);
}
char *nu_string_get_data(nu_string_t str)
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
void nu_string_append_format(nu_string_t *str, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_string_append_vformat(str, format, args);
    va_end(args);
}
void nu_string_append_vformat(nu_string_t *str, const char *format, va_list args)
{
    va_list args_temp;
    va_copy(args_temp, args);
    uint32_t n = vsnprintf(NULL, 0, format, args);
    va_end(args_temp);
    uint32_t len = nu_string_get_length_(*str);
    if (len + n > nu_string_get_capacity_(*str)) {
        nu_string_realloc_(str, len + n);
    }
    va_copy(args_temp, args);
    vsnprintf(nu_string_get_str_(*str) + len, n + 1, format, args);
    va_end(args_temp);
    nu_string_set_length_(str, n + len);
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
static inline uint32_t nu_string_find_first_ncstr(const char *str, uint32_t nstr, const char *token, uint32_t ntoken)
{
    NU_ASSERT(ntoken > 0);

    if (nstr < ntoken) return nstr;

    const char *s1 = str;
    const char *end = str + nstr - ntoken + 1;
    while (s1 != end) {
        if (!memcmp(s1++, token, ntoken)) {
            return s1 - str - 1;
        }
    }
    return nstr;
}
uint32_t nu_string_find_first_cstr(nu_string_t str, const char *token)
{
    return nu_string_find_first_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), token, strlen(token));
}
uint32_t nu_string_find_first(nu_string_t str, nu_string_t token)
{
    return nu_string_find_first_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), nu_string_get_str_(token), nu_string_get_length_(token));
}
static uint32_t nu_string_find_last_ncstr(const char *str, uint32_t nstr, const char *token, uint32_t ntoken)
{
    NU_ASSERT(ntoken > 0);

    if (nstr < ntoken) return nstr;

    const char *s1 = str + nstr - ntoken;
    const char *end = str - 1;
    while (s1 != end) {
        if (!memcmp(s1--, token, ntoken)) {
            return s1 - str + 1;
        }
    }
    return nstr;
}
uint32_t nu_string_cstr_find_first_cstr(const char *cstr, const char *token)
{
    return nu_string_find_last_ncstr(cstr, strlen(cstr), token, strlen(token));
}
uint32_t nu_string_cstr_find_first(const char *cstr, nu_string_t token)
{
    return nu_string_find_last_ncstr(cstr, strlen(cstr), nu_string_get_str_(token), nu_string_get_length_(token));
}
uint32_t nu_string_find_last_cstr(nu_string_t str, const char *token)
{
    return nu_string_find_last_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), token, strlen(token));
}
uint32_t nu_string_find_last(nu_string_t str, nu_string_t token)
{
    return nu_string_find_last_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), nu_string_get_str_(token), nu_string_get_length_(token));
}
static inline uint32_t nu_string_replace_ncstr(nu_string_t *str, const char *token, uint32_t ntoken, const char *other, uint32_t nother)
{
    if (ntoken > 0) {
        uint32_t nstr = nu_string_get_length_(*str);
        
        const char *cstr = nu_string_get_str_(*str);
        const char *first = cstr;
        uint32_t len = nstr;
        
        uint32_t count = 0;
        uint32_t index;
        while ((index = nu_string_find_first_ncstr(cstr, len, token, ntoken)) != len) {
            /* update index in the whole string view */
            index += (cstr - first);

            /* replace the token by other */
            nu_string_erase(str, index, ntoken);
            nu_string_insert_ncstr(str, other, nother, index);

            /* update string pointer and length */
            cstr = nu_string_get_cstr(*str) + (index + nother);
            len = nstr - index - ntoken;

            count++;
        }

        return count;
    } else {
        return 0;
    }
}
uint32_t nu_string_replace_cstr(nu_string_t *str, const char *token, const char *other)
{
    return nu_string_replace_ncstr(str, token, strlen(token), other, strlen(other));
}
uint32_t nu_string_replace(nu_string_t *str, const char *token, nu_string_t other)
{
    return nu_string_replace_ncstr(str, token, strlen(token), nu_string_get_str_(other), nu_string_get_length_(other));
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

    for (i = 0; i < len; i++) {
        /* find separator */
        if ((i <= (len - dlen)) && (memcmp(s + i, delim, dlen) == 0)) {
            tlen = i - start;
            if (tlen != 0) { /* valid token */
                nu_string_array_add_ncstr(tokens, s + start, tlen);
            }
            start = i = i + dlen;
        }
    }

    /* add final token */
    tlen = len - start;
    if (tlen > 0) {
        nu_string_array_add_ncstr(tokens, s + start, tlen);
    }
}
void nu_string_cstr_split(const char *cstr, const char *delim, nu_string_array_t tokens)
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
        header->data_capacity = (header->head + n + 1) * 2;
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
    header->data[header->head + n] = '\0';
    /* udpate meta data */
    header->head += n + 1;
    header->length++;
}
void nu_string_array_clear(nu_string_array_t array)
{
    nu_string_array_header_t *header = (nu_string_array_header_t*)array;
    header->length = 0;
    header->head = 0;
}

void nu_string_resolve_path(nu_string_t *path)
{
    nu_string_replace_cstr(path, "$ENGINE", NU_PATH_ENGINE_DIRECTORY);
    nu_string_replace_cstr(path, "$MODULE", NU_PATH_MODULE_DIRECTORY);
    nu_string_replace_cstr(path, "$ROOT", NU_PATH_ROOT_DIRECTORY);

    nu_string_replace_cstr(path, "\\", NU_PATH_SEPARATOR);
    nu_string_replace_cstr(path, "//", NU_PATH_SEPARATOR);
    if (nu_string_get_cstr(*path)[0] == NU_PATH_SEPARATOR[0]) {
        nu_string_erase(path, 0, 1);
    }
}
void nu_string_get_filename(nu_string_t path, nu_string_t *filename)
{
    nu_string_allocate_copy(filename, path);

    uint32_t index_back = nu_string_find_last_cstr(path, NU_PATH_SEPARATOR);
    if (index_back != nu_string_get_length(path)) {
        nu_string_erase(filename, 0, index_back + 1);
    }

    uint32_t index_dot = nu_string_find_last_cstr(*filename, ".");
    if (index_dot != nu_string_get_length(*filename)) {
        nu_string_erase(filename, index_dot, nu_string_get_length(*filename) - index_dot);
    }
}
void nu_string_get_directory(nu_string_t path, nu_string_t *directory)
{
    uint32_t index = nu_string_find_last_cstr(path, NU_PATH_SEPARATOR);
    if (index != nu_string_get_length(path)) {
        nu_string_allocate_substr(directory, path, 0, index + 1);
    } else {
        nu_string_allocate(directory);
    }
}
void nu_string_cstr_get_directory(const char *path, nu_string_t *directory)
{
    uint32_t index = nu_string_cstr_find_first_cstr(path, NU_PATH_SEPARATOR);
    if (index != strlen(path)) {
        nu_string_allocate_substr_cstr(directory, path, 0, index + 1);
    } else {
        nu_string_allocate(directory);
    }
}
void nu_string_get_extension(nu_string_t path, nu_string_t *extension)
{
    uint32_t index_dot = nu_string_find_last_cstr(path, ".");
    uint32_t index_back = nu_string_find_last_cstr(path, NU_PATH_SEPARATOR);
    if (index_dot == nu_string_get_length(path) ||
        (index_back != nu_string_get_length(path) && index_back > index_dot)) {
        nu_string_allocate(extension);
    } else {
        nu_string_allocate_substr(extension, path, index_dot + 1, nu_string_get_length(path) - index_dot - 1);
    }
}
bool nu_string_is_directory(nu_string_t path)
{
    uint32_t len = nu_string_get_length(path);
    if (len == 0) return true;
    char back = nu_string_get_cstr(path)[len - 1];
    return back == NU_PATH_SEPARATOR[0];
}
bool nu_string_is_filename(nu_string_t path)
{
    return !nu_string_is_directory(path);
}