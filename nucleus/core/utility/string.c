#include <nucleus/core/utility/string.h>

#include <nucleus/core/utility/container/vector.h>

nu_result_t nu_snprintf(char *str, uint32_t n, const char *format, ...)
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
uint32_t nu_strfirst(const char *str, uint32_t nstr, const char *token, uint32_t ntoken)
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
uint32_t nu_strlast(const char *str, uint32_t nstr, const char *token, uint32_t ntoken)
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
bool nu_strisdirectory(const char *path, uint32_t len)
{
    if (len == 0) return true;
    char back = path[len - 1];
    return back == NU_PATH_SEPARATOR[0];
}
bool nu_strisfilename(const char *path, uint32_t len)
{
    return !nu_strisdirectory(path, len);
}

// typedef struct {
//     nu_allocator_t allocator;
//     uint32_t length;
//     uint32_t capacity;
// } nu_string_header_t;

// static inline nu_string_header_t *nu_string_get_header_(nu_string_t str)
// {
//     return (nu_string_header_t*)str;
// }
// static inline char *nu_string_get_str_(nu_string_t str)
// {
//     return (char*)str + sizeof(nu_string_header_t);
// }
// static inline uint32_t nu_string_get_length_(nu_string_t str)
// {
//     return nu_string_get_header_(str)->length;
// }
// static inline uint32_t nu_string_get_capacity_(nu_string_t str)
// {
//     return nu_string_get_header_(str)->capacity;
// }
// static inline nu_string_t nu_string_allocate_(nu_allocator_t allocator, uint32_t capacity)
// {
//     nu_string_t str = (nu_string_t)nu_malloc(allocator, sizeof(nu_string_header_t) + capacity + 1);
//     nu_string_get_header_(str)->allocator = allocator;
//     nu_string_get_header_(str)->capacity  = capacity;
//     nu_string_get_header_(str)->length    = 0;
//     nu_string_get_str_(str)[0] = '\0';
//     return str;
// }
// static inline void nu_string_realloc_(nu_string_t *str, uint32_t capacity)
// {
//     *str = (nu_string_t)nu_realloc(nu_string_get_header_(*str)->allocator, *str, sizeof(nu_string_header_t) + capacity + 1);
//     nu_string_get_header_(*str)->capacity = capacity;
// }
// static inline void nu_string_set_length_(nu_string_t *str, uint32_t len)
// {
//     nu_string_get_header_(*str)->length = len;
//     nu_string_get_str_(*str)[len] = '\0';
// }

// nu_string_t nu_string_allocate(nu_allocator_t allocator)
// {    
//     return nu_string_allocate_(allocator, 0);
// }
// nu_string_t nu_string_allocate_capacity(nu_allocator_t allocator, uint32_t capacity)
// {
//     return nu_string_allocate_(allocator, capacity);
// }
// nu_string_t nu_string_allocate_length(nu_allocator_t allocator, uint32_t length)
// {
//     nu_string_t str = nu_string_allocate_(allocator, length);
//     nu_string_set_length_(&str, length);
//     return str;
// }
// static inline nu_string_t nu_string_allocate_ncstr(nu_allocator_t allocator, const char *cstr, uint32_t n)
// {
//     nu_string_t str = nu_string_allocate_(allocator, n);
//     memcpy(nu_string_get_str_(str), cstr, n);
//     nu_string_set_length_(&str, n);
//     return str;
// }
// nu_string_t nu_string_allocate_cstr(nu_allocator_t allocator, const char *cstr)
// {
//     return nu_string_allocate_ncstr(allocator, cstr, strlen(cstr));
// }
// nu_string_t nu_string_allocate_copy(nu_allocator_t allocator, nu_string_t other)
// {
//     return nu_string_allocate_ncstr(allocator, nu_string_get_str_(other), nu_string_get_length_(other));
// }
// static inline nu_string_t nu_string_allocate_substr_ncstr(nu_allocator_t allocator, const char *cstr, uint32_t n, uint32_t index, uint32_t len)
// {
//     NU_ASSERT(index < n);
//     NU_ASSERT(n > 0 && index + len <= n);
//     len = NU_MIN(index + len, n);
//     nu_string_t str = nu_string_allocate_(allocator, len);
//     memcpy(nu_string_get_str_(str), cstr + index, len);
//     nu_string_set_length_(&str, len);
//     return str;
// }
// nu_string_t nu_string_allocate_substr_cstr(nu_allocator_t allocator, const char *cstr, uint32_t index, uint32_t len)
// {
//     return nu_string_allocate_substr_ncstr(allocator, cstr, strlen(cstr), index, len);
// }
// nu_string_t nu_string_allocate_substr(nu_allocator_t allocator, nu_string_t other, uint32_t index, uint32_t len)
// {
//     return nu_string_allocate_substr_ncstr(allocator, nu_string_get_str_(other), nu_string_get_length_(other), index, len);
// }
// nu_string_t nu_string_allocate_format(nu_allocator_t allocator, const char *format, ...)
// {
//     va_list args;
//     va_start(args, format);
//     nu_string_t str = nu_string_allocate_vformat(allocator, format, args);
//     va_end(args);
//     return str;
// }
// nu_string_t nu_string_allocate_vformat(nu_allocator_t allocator, const char *format, va_list args)
// {
//     va_list args_temp;
//     va_copy(args_temp, args);
//     uint32_t n = vsnprintf(NULL, 0, format, args);
//     va_end(args_temp);
//     nu_string_t str = nu_string_allocate_(allocator, n);
//     va_copy(args_temp, args);
//     vsnprintf(nu_string_get_str_(str), n + 1, format, args);
//     va_end(args_temp);
//     nu_string_set_length_(&str, n);
//     return str;
// }
// void nu_string_free(nu_string_t str)
// {
//     nu_free(nu_string_get_header_(str)->allocator, str);
// }
// void nu_string_clear(nu_string_t *str)
// {
//     nu_string_set_length_(str, 0);
// }
// const char *nu_string_get_cstr(nu_string_t str)
// {
//     return nu_string_get_str_(str);
// }
// char *nu_string_get_data(nu_string_t str)
// {
//     return nu_string_get_str_(str);
// }
// uint32_t nu_string_get_length(nu_string_t str)
// {
//     return nu_string_get_length_(str);
// }
// static inline void nu_string_set_ncstr(nu_string_t *str, const char *cstr, uint32_t n)
// {
//     uint32_t capacity = nu_string_get_capacity_(*str);
//     if (n > capacity) {
//         nu_string_realloc_(str, n);
//     }
//     memcpy(nu_string_get_str_(*str), cstr, n);
//     nu_string_set_length_(str, n);
// }
// void nu_string_set_cstr(nu_string_t *str, const char *cstr)
// {
//     nu_string_set_ncstr(str, cstr, strlen(cstr));
// }
// void nu_string_set(nu_string_t *str, nu_string_t other)
// {
//     NU_ASSERT(*str != other);
//     nu_string_set_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other));
// }
// static inline void nu_string_append_ncstr(nu_string_t *str, const char *cstr, uint32_t n)
// {
//     if (n > 0) {
//         uint32_t len = nu_string_get_length_(*str);
//         if (len + n > nu_string_get_capacity_(*str)) {
//             nu_string_realloc_(str, len + n);
//         }
//         memcpy(nu_string_get_str_(*str) + len, cstr, n);
//         nu_string_set_length_(str, len + n);
//     }
// }
// void nu_string_append_cstr(nu_string_t *str, const char *cstr)
// {
//     nu_string_append_ncstr(str, cstr, strlen(cstr));
// }
// void nu_string_append(nu_string_t *str, nu_string_t other)
// {
//     NU_ASSERT(*str != other);
//     nu_string_append_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other));
// }
// void nu_string_append_format(nu_string_t *str, const char *format, ...)
// {
//     va_list args;
//     va_start(args, format);
//     nu_string_append_vformat(str, format, args);
//     va_end(args);
// }
// void nu_string_append_vformat(nu_string_t *str, const char *format, va_list args)
// {
//     va_list args_temp;
//     va_copy(args_temp, args);
//     uint32_t n = vsnprintf(NULL, 0, format, args);
//     va_end(args_temp);
//     uint32_t len = nu_string_get_length_(*str);
//     if (len + n > nu_string_get_capacity_(*str)) {
//         nu_string_realloc_(str, len + n);
//     }
//     va_copy(args_temp, args);
//     vsnprintf(nu_string_get_str_(*str) + len, n + 1, format, args);
//     va_end(args_temp);
//     nu_string_set_length_(str, n + len);
// }
// static inline void nu_string_insert_ncstr(nu_string_t *str, const char *cstr, uint32_t n, uint32_t index)
// {
//     if (n > 0) {
//         uint32_t len = nu_string_get_length_(*str);
//         NU_ASSERT(index <= len);
//         if (len + n > nu_string_get_capacity_(*str)) {
//             nu_string_realloc_(str, len + n);
//         }
//         memmove(nu_string_get_str_(*str) + index + n, nu_string_get_str_(*str) + index, 
//             len - index + 1);
//         memcpy(nu_string_get_str_(*str) + index, cstr, n);
//         nu_string_set_length_(str, len + n);
//     }
// }
// void nu_string_insert_cstr(nu_string_t *str, const char *cstr, uint32_t index)
// {
//     nu_string_insert_ncstr(str, cstr, strlen(cstr), index);
// }
// void nu_string_insert(nu_string_t *str, nu_string_t other, uint32_t index)
// {
//     NU_ASSERT(*str != other);
//     nu_string_insert_ncstr(str, nu_string_get_str_(other), nu_string_get_length_(other), index);
// }
// static inline uint32_t nu_string_find_first_ncstr(const char *str, uint32_t nstr, const char *token, uint32_t ntoken)
// {
//     NU_ASSERT(ntoken > 0);

//     if (nstr < ntoken) return nstr;

//     const char *s1 = str;
//     const char *end = str + nstr - ntoken + 1;
//     while (s1 != end) {
//         if (!memcmp(s1++, token, ntoken)) {
//             return s1 - str - 1;
//         }
//     }
//     return nstr;
// }
// uint32_t nu_string_find_first_cstr(nu_string_t str, const char *token)
// {
//     return nu_string_find_first_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), token, strlen(token));
// }
// uint32_t nu_string_find_first(nu_string_t str, nu_string_t token)
// {
//     return nu_string_find_first_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), nu_string_get_str_(token), nu_string_get_length_(token));
// }
// static uint32_t nu_string_find_last_ncstr(const char *str, uint32_t nstr, const char *token, uint32_t ntoken)
// {
//     NU_ASSERT(ntoken > 0);

//     if (nstr < ntoken) return nstr;

//     const char *s1 = str + nstr - ntoken;
//     const char *end = str - 1;
//     while (s1 != end) {
//         if (!memcmp(s1--, token, ntoken)) {
//             return s1 - str + 1;
//         }
//     }
//     return nstr;
// }
// uint32_t nu_string_cstr_find_first_cstr(const char *cstr, const char *token)
// {
//     return nu_string_find_last_ncstr(cstr, strlen(cstr), token, strlen(token));
// }
// uint32_t nu_string_cstr_find_first(const char *cstr, nu_string_t token)
// {
//     return nu_string_find_last_ncstr(cstr, strlen(cstr), nu_string_get_str_(token), nu_string_get_length_(token));
// }
// uint32_t nu_string_find_last_cstr(nu_string_t str, const char *token)
// {
//     return nu_string_find_last_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), token, strlen(token));
// }
// uint32_t nu_string_find_last(nu_string_t str, nu_string_t token)
// {
//     return nu_string_find_last_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), nu_string_get_str_(token), nu_string_get_length_(token));
// }
// static inline uint32_t nu_string_replace_ncstr(nu_string_t *str, const char *token, uint32_t ntoken, const char *other, uint32_t nother)
// {
//     if (ntoken > 0) {
//         uint32_t nstr = nu_string_get_length_(*str);
        
//         const char *cstr = nu_string_get_str_(*str);
//         const char *first = cstr;
//         uint32_t len = nstr;
        
//         uint32_t count = 0;
//         uint32_t index;
//         while ((index = nu_string_find_first_ncstr(cstr, len, token, ntoken)) != len) {
            
//             // Update index in the whole string view
//             index += (cstr - first);

//             // Replace the token by other
//             nu_string_erase(str, index, ntoken);
//             nu_string_insert_ncstr(str, other, nother, index);

//             // Update string pointer and length
//             cstr = nu_string_get_cstr(*str) + (index + nother);
//             len = nstr - index - ntoken;

//             count++;
//         }

//         return count;
//     } else {
//         return 0;
//     }
// }
// uint32_t nu_string_replace_cstr(nu_string_t *str, const char *token, const char *other)
// {
//     return nu_string_replace_ncstr(str, token, strlen(token), other, strlen(other));
// }
// uint32_t nu_string_replace(nu_string_t *str, const char *token, nu_string_t other)
// {
//     return nu_string_replace_ncstr(str, token, strlen(token), nu_string_get_str_(other), nu_string_get_length_(other));
// }
// void nu_string_erase(nu_string_t *str, uint32_t index, uint32_t len)
// {
//     if (len > 0) {
//         NU_ASSERT(index < nu_string_get_length_(*str));
//         NU_ASSERT(index + len <= nu_string_get_length_(*str));
//         memmove(nu_string_get_str_(*str) + index, nu_string_get_str_(*str) + index + len,
//             nu_string_get_length_(*str) - len - index);
//         nu_string_set_length_(str, nu_string_get_length_(*str) - len);
//     }
// }
// void nu_string_trim(nu_string_t *str)
// {
//     uint32_t len = nu_string_get_length_(*str);
//     char *pstr = nu_string_get_str_(*str);
//     if (len == 0) return;

//     char *pstart = pstr;

//     // Trim leading space
//     while (isspace(*pstart)) pstart++;

//     // All spaces ?
//     if (*pstart == '\0') {
//         nu_string_realloc_(str, 0);
//         return;
//     }

//     // Trim trailing space
//     char *pend = pstr + len - 1;
//     while (pend > pstart && isspace(*pend)) pend--;

//     uint32_t length = pend - pstart + 1;
//     uint32_t index  = pstart - pstr;

//     memmove(nu_string_get_str_(*str), nu_string_get_str_(*str) + index, length);
//     nu_string_set_length_(str, length);
// }
// static inline void nu_string_split_ncstr(const char *cstr, uint32_t n, const char *delim, nu_string_array_t tokens)
// {
//     uint32_t start, i, len, dlen, tlen;
//     const char *s;

//     start = i = 0;
//     len = n;
//     s = cstr;
//     dlen = strlen(delim);

//     nu_string_array_clear(tokens);

//     if (len == 0) return;

//     for (i = 0; i < len; i++) {
//         // Find separator
//         if ((i <= (len - dlen)) && (memcmp(s + i, delim, dlen) == 0)) {
//             tlen = i - start;
//             if (tlen != 0) { // Valid token
//                 nu_string_array_add_ncstr(tokens, s + start, tlen);
//             }
//             start = i = i + dlen;
//         }
//     }

//     // Add final token
//     tlen = len - start;
//     if (tlen > 0) {
//         nu_string_array_add_ncstr(tokens, s + start, tlen);
//     }
// }
// void nu_string_cstr_split(const char *cstr, const char *delim, nu_string_array_t tokens)
// {
//     nu_string_split_ncstr(cstr, strlen(cstr), delim, tokens);
// }
// void nu_string_split(nu_string_t str, const char *delim, nu_string_array_t tokens)
// {
//     nu_string_split_ncstr(nu_string_get_str_(str), nu_string_get_length_(str), delim, tokens);
// }

// typedef struct {
//     nu_allocator_t allocator;
//     uint32_t data_capacity;
//     uint32_t index_capacity;
//     uint32_t head;
//     uint32_t length;
//     char *data;
//     uint32_t *indexes;
// } nu_string_array_header_t;

// #define NU_STRING_ARRAY_INITIAL_CAPACITY 20

// nu_string_array_t nu_string_array_allocate(nu_allocator_t allocator)
// {
//     nu_string_array_header_t *header = (nu_string_array_header_t*)nu_malloc(allocator, sizeof(nu_string_array_header_t));
//     header->allocator      = allocator;
//     header->data_capacity  = NU_STRING_ARRAY_INITIAL_CAPACITY;
//     header->index_capacity = NU_STRING_ARRAY_INITIAL_CAPACITY;
//     header->head           = 0;
//     header->length         = 0;
//     header->data           = (char*)nu_malloc(allocator, sizeof(char) * header->data_capacity);
//     header->indexes        = (uint32_t*)nu_malloc(allocator, sizeof(uint32_t) * header->index_capacity);
//     return (nu_string_array_t)header;
// }
// void nu_string_array_free(nu_string_array_t array)
// {
//     nu_string_array_header_t *header = (nu_string_array_header_t*)array;
//     nu_free(header->allocator, header->data);
//     nu_free(header->allocator, header->indexes);
//     nu_free(header->allocator, array);
// }
// const char *nu_string_array_get(nu_string_array_t array, uint32_t index)
// {
//     nu_string_array_header_t *header = (nu_string_array_header_t*)array;
//     NU_ASSERT(index < header->length);
//     return header->data + header->indexes[index];
// }
// uint32_t nu_string_array_get_length(nu_string_array_t array)
// {
//     nu_string_array_header_t *header = (nu_string_array_header_t*)array;
//     return header->length;
// }
// void nu_string_array_add_ncstr(nu_string_array_t array, const char *cstr, uint32_t n)
// {
//     nu_string_array_header_t *header = (nu_string_array_header_t*)array;
//     // Realloc data
//     if (header->head + n + 1 > header->data_capacity) {
//         header->data_capacity = (header->head + n + 1) * 2;
//         header->data = (char*)nu_realloc(header->allocator, header->data, header->data_capacity);
//     }
//     // Realloc indexes
//     if (header->length >= header->index_capacity) {
//         header->index_capacity *= 2;
//         header->indexes = (uint32_t*)nu_realloc(header->allocator, header->indexes, header->index_capacity);
//     }
//     // Save index
//     header->indexes[header->length] = header->head;
//     // Copy the cstr
//     memcpy(header->data + header->head, cstr, n);
//     header->data[header->head + n] = '\0';
//     // Udpate meta data
//     header->head += n + 1;
//     header->length++;
// }
// void nu_string_array_clear(nu_string_array_t array)
// {
//     nu_string_array_header_t *header = (nu_string_array_header_t*)array;
//     header->length = 0;
//     header->head = 0;
// }

// void nu_string_resolve_path(nu_string_t *path)
// {
//     nu_string_replace_cstr(path, "$ENGINE", NU_PATH_ENGINE_DIRECTORY);
//     nu_string_replace_cstr(path, "$MODULE", NU_PATH_MODULE_DIRECTORY);
//     nu_string_replace_cstr(path, "$ROOT", NU_PATH_ROOT_DIRECTORY);

//     nu_string_replace_cstr(path, "\\", NU_PATH_SEPARATOR);
//     nu_string_replace_cstr(path, "//", NU_PATH_SEPARATOR);
//     if (nu_string_get_cstr(*path)[0] == NU_PATH_SEPARATOR[0]) {
//         nu_string_erase(path, 0, 1);
//     }
// }
// nu_string_t nu_string_allocate_path_filename(nu_allocator_t allocator, nu_string_t path)
// {
//     nu_string_t filename = nu_string_allocate_copy(allocator, path);

//     uint32_t index_back = nu_string_find_last_cstr(path, NU_PATH_SEPARATOR);
//     if (index_back != nu_string_get_length(path)) {
//         nu_string_erase(&filename, 0, index_back + 1);
//     }

//     uint32_t index_dot = nu_string_find_last_cstr(filename, ".");
//     if (index_dot != nu_string_get_length(filename)) {
//         nu_string_erase(&filename, index_dot, nu_string_get_length(filename) - index_dot);
//     }

//     return filename;
// }
// nu_string_t nu_string_allocate_path_directory(nu_allocator_t allocator, nu_string_t path)
// {
//     uint32_t index = nu_string_find_last_cstr(path, NU_PATH_SEPARATOR);
//     if (index != nu_string_get_length(path)) {
//         return nu_string_allocate_substr(allocator, path, 0, index + 1);
//     } else {
//         return nu_string_allocate(allocator);
//     }
// }
// nu_string_t nu_string_allocate_cstr_directory(nu_allocator_t allocator, const char *path)
// {
//     uint32_t index = nu_string_cstr_find_first_cstr(path, NU_PATH_SEPARATOR);
//     if (index != strlen(path)) {
//         return nu_string_allocate_substr_cstr(allocator, path, 0, index + 1);
//     } else {
//         return nu_string_allocate(allocator);
//     }
// }
// nu_string_t nu_string_allocate_path_extension(nu_allocator_t allocator, nu_string_t path)
// {
//     uint32_t index_dot = nu_string_find_last_cstr(path, ".");
//     uint32_t index_back = nu_string_find_last_cstr(path, NU_PATH_SEPARATOR);
//     if (index_dot == nu_string_get_length(path) ||
//         (index_back != nu_string_get_length(path) && index_back > index_dot)) {
//         return nu_string_allocate(allocator);
//     } else {
//         return nu_string_allocate_substr(allocator, path, index_dot + 1, nu_string_get_length(path) - index_dot - 1);
//     }
// }
// bool nu_string_is_directory(nu_string_t path)
// {
//     uint32_t len = nu_string_get_length(path);
//     if (len == 0) return true;
//     char back = nu_string_get_cstr(path)[len - 1];
//     return back == NU_PATH_SEPARATOR[0];
// }
// bool nu_string_is_filename(nu_string_t path)
// {
//     return !nu_string_is_directory(path);
// }

nu_string_t nu_string_allocate_capacity_(nu_allocator_t allocator, uint32_t capacity, const char *file, uint32_t line)
{
    nu_vector(char) str = nu_vector_allocate_capacity_(allocator, sizeof(char), capacity + 1, file, line);
    nu_vector_push_value(&str, '\0');
    return (nu_string_t)str;
}
nu_string_t nu_string_allocate_(nu_allocator_t allocator, const char *s, uint32_t len, const char *file, uint32_t line)
{
    nu_vector(char) str = nu_string_allocate_capacity_(allocator, len + 1, file, line);
    nu_vector_resize_((nu_vector_t*)&str, len + 1, sizeof(char),  file, line);
    memcpy(str, s, len);
    str[len] = '\0';
    return (nu_string_t)str;
}
nu_string_t nu_string_allocate_format_(nu_allocator_t allocator, const char *file, uint32_t line, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_string_t str = nu_string_allocate_vformat_(allocator, format, args, file, line);
    va_end(args);
    return str;
}
nu_string_t nu_string_allocate_vformat_(nu_allocator_t allocator, const char *format, va_list args, const char *file, uint32_t line)
{
    va_list args_temp;
    va_copy(args_temp, args);
    uint32_t n = vsnprintf(NULL, 0, format, args); // n doesn't contains the end character
    va_end(args_temp);
    nu_string_t str = nu_string_allocate_capacity_(allocator, n, file, line);
    nu_vector_resize_((nu_vector_t*)&str, n + 1, sizeof(char), file, line);
    va_copy(args_temp, args);
    vsnprintf(str, n + 1, format, args);
    va_end(args_temp);
    return str;
}
void nu_string_free_(nu_string_t str, const char *file, uint32_t line)
{
    nu_vector_free_(str, file, line);
}
nu_string_t nu_string_clear(nu_string_t *str)
{
    nu_vector_resize_((nu_vector_t*)str, 1, sizeof(char), __FILE__, __LINE__);
    (*str)[0] = '\0';
    return *str;
}
uint32_t nu_string_size(const nu_string_t str)
{
    return nu_vector_size(str) - 1;
}
nu_string_t nu_string_append_(nu_string_t *pstr, const char *s, uint32_t len, const char *file, uint32_t line)
{
    if (len > 0) {
        nu_vector(char) vec = (*pstr);
        uint32_t size = nu_vector_size(vec);
        nu_vector_resize_((nu_vector_t*)&vec, size + len, sizeof(char), file, line);
        memcpy(vec + size - 1, s, len);
        vec[size + len - 1] = '\0';
        (*pstr) = vec;
    }
    return *pstr;
}
nu_string_t nu_string_append_format_(nu_string_t *str, const char *file, uint32_t line, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_string_t ostr = nu_string_append_vformat_(str, format, args, file, line);
    va_end(args);
    return ostr;
}
nu_string_t nu_string_append_vformat_(nu_string_t *pstr, const char *format, va_list args, const char *file, uint32_t line)
{
    va_list args_temp;
    va_copy(args_temp, args);
    uint32_t n = vsnprintf(NULL, 0, format, args);
    va_end(args_temp);
    nu_vector(char) vec = *pstr;
    uint32_t size = nu_vector_size(vec);
    nu_vector_resize_((nu_vector_t*)&vec, size + n, sizeof(char), file, line);
    va_copy(args_temp, args);
    vsnprintf(vec + n, n, format, args);
    va_end(args_temp);
    *pstr = vec;
    return *pstr;
}
nu_string_t nu_string_insert_(nu_string_t *pstr, const char *s, uint32_t len, uint32_t index, const char *file, uint32_t line)
{
    if (len > 0) {
        nu_vector(char) vec  = *pstr;
        uint32_t size = nu_vector_size(vec);
        NU_ASSERT(index <= len);
        nu_vector_resize_((nu_vector_t*)&vec, size + len, sizeof(char), file, line);
        memmove(vec + index + size, vec + index, len - index + 1);
        memcpy(vec + index, s, len);
        *pstr = vec;
    }
    return *pstr;
}
uint32_t nu_string_find_first_(const nu_string_t str, const char *token, uint32_t ntoken)
{
    return nu_strfirst(str, nu_string_size(str), token, ntoken);
}
uint32_t nu_string_find_last_(const nu_string_t str, const char *token, uint32_t ntoken)
{
    return nu_strlast(str, nu_string_size(str), token, ntoken);   
}
uint32_t nu_string_replace_(nu_string_t *str, const char *token, uint32_t ntoken, const char *other, uint32_t nother, const char *file, uint32_t line)
{
    if (ntoken > 0) {
        uint32_t nstr = nu_string_size(*str);
        
        const char *cstr = *str;
        const char *first = cstr;
        uint32_t len = nstr;
        
        uint32_t count = 0;
        uint32_t index;
        while ((index = nu_strfirst(cstr, len, token, ntoken)) != len) {
            
            // Update index in the whole string view
            index += (cstr - first);

            // Replace the token by other
            nu_string_erase(str, index, ntoken);
            nu_string_insert_(str, other, nother, index, file, line);

            // Update string pointer and length
            cstr = (*str) + (index + nother);
            len = nstr - index - ntoken;

            count++;
        }

        return count;
    } else {
        return 0;
    }
}
nu_string_t nu_string_erase(nu_string_t *pstr, uint32_t index, uint32_t len)
{
    if (len > 0) {
        NU_ASSERT(index < nu_string_size(*pstr));
        NU_ASSERT(index + len <= nu_string_size(*pstr));
        memmove((*pstr) + index, (*pstr) + index + len,
            nu_string_size(*pstr) - len - index);
        (*pstr)[nu_string_size(*pstr) - len] = '\0';
        nu_vector(char) str = (*pstr); 
        nu_vector_resize_((nu_vector_t*)&str, nu_vector_size(str) - len, sizeof(char), __FILE__, __LINE__);
        *pstr = str;
    }
    return *pstr;
}
nu_string_t nu_string_trim(nu_string_t *str)
{
    uint32_t len = nu_string_size(*str);
    char *pstr = (*str);
    if (len == 0) return *str;

    char *pstart = pstr;

    // Trim leading space
    while (isspace(*pstart)) pstart++;

    // All spaces ?
    nu_vector(char) vec = *str;
    if (*pstart == '\0') {
        nu_vector_resize_((nu_vector_t*)&vec, 1, sizeof(char), __FILE__, __LINE__);
        *str = vec;
        return *str;
    }

    // Trim trailing space
    char *pend = pstr + len - 1;
    while (pend > pstart && isspace(*pend)) pend--;

    uint32_t length = pend - pstart + 1;
    uint32_t index  = pstart - pstr;

    memmove((*str), (*str) + index, length);
    nu_vector_resize_((nu_vector_t*)&vec, length, sizeof(char), __FILE__, __LINE__);
    return *str;
}
nu_string_t nu_string_set_(nu_string_t *str, const char *s, uint32_t len, const char *file, uint32_t line)
{
    nu_vector(char) vec = *str;
    nu_vector_resize_((nu_vector_t*)&vec, len + 1, sizeof(char), file, line);
    memcpy(vec, s, len);
    vec[len] = '\0';
    return (*str = vec);
}

#define NU_ENGINE_TOKEN "$ENGINE"
#define NU_MODULE_TOKEN "$MODULE"
#define NU_ROOT_TOKEN   "$ROOT"
#define NU_WINDOWS_SLASH "//"
#define NU_WINDOWS_BSLASH "\\"

nu_string_t nu_string_resolve_path_(nu_string_t *pstr, const char *file, uint32_t line)
{
    nu_string_replace_(pstr, NU_ENGINE_TOKEN, strlen(NU_ENGINE_TOKEN), NU_PATH_ENGINE_DIRECTORY, strlen(NU_PATH_ENGINE_DIRECTORY), file, line);
    nu_string_replace_(pstr, NU_MODULE_TOKEN, strlen(NU_MODULE_TOKEN), NU_PATH_MODULE_DIRECTORY, strlen(NU_PATH_MODULE_DIRECTORY), file, line);
    nu_string_replace_(pstr, NU_ROOT_TOKEN, strlen(NU_ROOT_TOKEN), NU_PATH_ROOT_DIRECTORY, strlen(NU_PATH_ROOT_DIRECTORY), file, line);

    nu_string_replace_(pstr, NU_WINDOWS_SLASH, strlen(NU_WINDOWS_SLASH), NU_PATH_SEPARATOR, strlen(NU_PATH_SEPARATOR), file, line);
    nu_string_replace_(pstr, NU_WINDOWS_BSLASH, strlen(NU_WINDOWS_BSLASH), NU_PATH_SEPARATOR, strlen(NU_PATH_SEPARATOR), file, line);
    if ((*pstr)[0] == NU_PATH_SEPARATOR[0]) {
        nu_string_erase(pstr, 0, 1);
    }
    return *pstr;
}
nu_string_t nu_string_allocate_filename_(nu_allocator_t allocator, const char *path, const char *file, uint32_t line)
{
    uint32_t npath = strlen(path);
    nu_string_t filename = nu_string_allocate_(allocator, path, npath, file, line);

    uint32_t index_back = nu_strlast(path, npath, NU_PATH_SEPARATOR, strlen(NU_PATH_SEPARATOR));
    if (index_back != npath) {
        nu_string_erase(&filename, 0, index_back + 1);
    }

    uint32_t index_dot = nu_string_find_last(filename, ".");
    if (index_dot != nu_string_size(filename)) {
        nu_string_erase(&filename, index_dot, nu_string_size(filename) - index_dot);
    }

    return filename;
}
nu_string_t nu_string_allocate_directory_(nu_allocator_t allocator, const char *path, const char *file, uint32_t line)
{
    uint32_t npath = strlen(path);
    uint32_t index = nu_strlast(path, npath, NU_PATH_SEPARATOR, strlen(NU_PATH_SEPARATOR));
    if (index != npath) {
        return nu_string_allocate_(allocator, path, index + 1, file, line);
    } else {
        return nu_string_allocate_capacity_(allocator, 0, file, line);
    }
}
nu_string_t nu_string_allocate_extension_(nu_allocator_t allocator, const char *path, const char *file, uint32_t line)
{
    uint32_t npath = strlen(path);
    uint32_t index_dot = nu_strlast(path, npath, ".", 1);
    uint32_t index_back = nu_strlast(path, npath, NU_PATH_SEPARATOR, 1);
    if (index_dot == npath ||
        (index_back != npath && index_back > index_dot)) {
        return nu_string_allocate_capacity_(allocator, 0, file, line);
    } else {
        return nu_string_allocate_(allocator, path + index_dot + 1, npath - index_dot - 1, file, line);
    }
}