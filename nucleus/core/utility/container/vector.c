#include <nucleus/core/utility/container/vector.h>

#include <nucleus/core/system/allocator/api.h>

#define GROWTH_FACTOR 1.3

typedef struct {
    nu_allocator_t allocator;
    uint32_t size;
    uint32_t capacity;
} nu_vector_header_t;

static inline void nu_vector_swap_block(uint8_t *a, uint8_t *b, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        uint8_t aa = a[i];
        a[i] = b[i];
        b[i] = aa;
    }
}
static inline nu_vector_header_t *nu_vector_header_(nu_vector_t vec)
{
    return (nu_vector_header_t*)((uint8_t*)vec - sizeof(nu_vector_header_t));
}
static inline nu_vector_t nu_vector_data_(nu_vector_header_t *header)
{
    return (nu_vector_t)((uint8_t*)header + sizeof(nu_vector_header_t));
}

nu_vector_t nu_vector_allocate_capacity_(nu_allocator_t allocator, size_t osize, uint32_t capacity, const char *file, uint32_t line)
{
    void *data = nu_malloc_(allocator, sizeof(nu_vector_header_t) + capacity * osize, file, line);
    nu_vector_t vec = (uint8_t*)data + sizeof(nu_vector_header_t);
    nu_vector_header_t *header = (nu_vector_header_t*)data;
    header->allocator = allocator;
    header->capacity  = capacity;
    header->size      = 0;
    return vec;
}
void nu_vector_free_(nu_vector_t vec, const char *file, uint32_t line)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    nu_free_(header->allocator, header, file, line);
}
void nu_vector_clear(nu_vector_t vec)
{
    nu_vector_header_(vec)->size = 0;
}
bool nu_vector_empty(nu_vector_t vec)
{
    return nu_vector_header_(vec)->size == 0;
}
void *nu_vector_last_(const nu_vector_t vec, size_t size)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    if (header->size == 0) return NULL;
    return (uint8_t*)(vec) + size * (header->size - 1);
}
bool nu_vector_find_index_(const nu_vector_t vec, nu_vector_eq_pfn_t eq, const void *user, uint32_t *index, size_t size)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    for (uint32_t i = 0; i < header->size; i++) {
        if (eq((uint8_t*)vec + size * i, user)) {
            *index = i;
            return true;
        }
    }
    return false;
}
void *nu_vector_find_(const nu_vector_t vec, nu_vector_eq_pfn_t eq, const void *user, size_t size)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    for (uint32_t i = 0; i < header->size; i++) {
        uint8_t *data = (uint8_t*)vec + size * i;
        if (eq(data, user)) {
            return data;
        }
    }
    return NULL;
}
uint32_t nu_vector_size(const nu_vector_t vec)
{
    return nu_vector_header_(vec)->size;
}
uint32_t nu_vector_capacity(const nu_vector_t vec)
{
    return nu_vector_header_(vec)->capacity;
}
uint32_t nu_vector_allocated_memory_(const nu_vector_t vec, size_t size)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    return sizeof(nu_vector_header_t) + size * header->capacity;
}
void *nu_vector_push_(nu_vector_t *vec, size_t size, const char *file, uint32_t line)
{
    nu_vector_header_t *header = nu_vector_header_(*vec);
    header->size++;
    if (header->size > header->capacity) {
        header->capacity = (uint32_t)((float)header->size * GROWTH_FACTOR);
        header = nu_realloc_(header->allocator, header, sizeof(nu_vector_header_t) + header->capacity * size, file, line);
        *vec = (nu_vector_t)((size_t)header + sizeof(nu_vector_header_t));
    }
    return (void*)((size_t)(*vec) + size * (header->size - 1));
}
bool nu_vector_pop(nu_vector_t vec)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    if (header->size) {
        header->size--;
        return true;
    }
    return false;
}
void nu_vector_resize_(nu_vector_t *vec, uint32_t newsize, size_t size, const char *file, uint32_t line)
{
    nu_vector_header_t *header = nu_vector_header_(*vec);
    header->size = newsize;
    if (header->size > header->capacity) {
        header->capacity = newsize;
        header = nu_realloc_(header->allocator, header, sizeof(nu_vector_header_t) + size * header->capacity, file, line);
        *vec = nu_vector_data_(header);
    }
}
void nu_vector_swap_(nu_vector_t vec, uint32_t first, uint32_t second, size_t size)
{
    NU_ASSERT(first < nu_vector_header_(vec)->size);
    NU_ASSERT(second < nu_vector_header_(vec)->size);
    if (first != second) {
        uint8_t *dfirst  = (uint8_t*)vec + first * size;
        uint8_t *dsecond = (uint8_t*)vec + second * size;
        nu_vector_swap_block(dfirst, dsecond, size);
    }
}
void nu_vector_swap_last_(nu_vector_t vec, uint32_t i, size_t size)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    nu_vector_swap_(vec, i, header->size - 1, size);
}
void nu_vector_swap_last_pop_(nu_vector_t vec, uint32_t i, size_t size)
{
    nu_vector_swap_last_(vec, i, size);
    nu_vector_pop(vec);
}
void *nu_vector_insert_(nu_vector_t *vec, uint32_t i, size_t size, const char *file, uint32_t line)
{
    nu_vector_push_(vec, size, file, line);
    nu_vector_header_t *header = nu_vector_header_(*vec);
    NU_ASSERT(i < header->size);
    if (i < header->size - 1) {
        size_t length_to_move = header->size - i - 1; 
        void *src = (uint8_t*)(*vec) + i * size; 
        void *dst = (uint8_t*)src + size;
        memmove(dst, src, size * length_to_move);
    }
    return (uint8_t*)(*vec) + i * size;
}
void nu_vector_erase_(nu_vector_t vec, uint32_t i, size_t size)
{
    nu_vector_header_t *header = nu_vector_header_(vec);
    NU_ASSERT(i < header->size);
    if (i != header->size - 1) {
        size_t length_to_move = header->size - i - 1; 
        void *src = (void*)((size_t)vec + ((size_t)i + 1) * size); 
        void *dst = (void*)((size_t)src - size);
        memmove(dst, src, size * length_to_move);
    }
    nu_vector_pop(vec);
}

bool nu_vector_eq_int32(const void *a, const void *b)
{
    return *((int32_t*)a) == *((int32_t*)b);
}