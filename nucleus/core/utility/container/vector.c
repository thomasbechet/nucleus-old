#include <nucleus/core/utility/container/vector.h>

#include <nucleus/core/coresystem/memory/memory.h>

#define VECTOR_DEFAULT_CAPACITY 10

typedef struct {
    uint32_t size;
    uint32_t capacity;
    uint32_t object_size;
} nu_vector_header_t;

static inline void nu_vector_swap_block(uint8_t *a, uint8_t *b, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        uint8_t aa = a[i];
        a[i] = b[i];
        b[i] = aa;
    }
}
static inline nu_vector_header_t *nu_vector_get_header_(nu_vector_t vec)
{
    return (nu_vector_header_t*)((uint8_t*)vec - sizeof(nu_vector_header_t));
}
static inline nu_vector_t nu_vector_get_data_(nu_vector_header_t *header)
{
    return (nu_vector_t)((uint8_t*)header + sizeof(nu_vector_header_t));
}

void nu_vector_allocate_(nu_vector_t *vec, size_t object_size)
{
    void *data = nu_malloc(sizeof(nu_vector_header_t) + VECTOR_DEFAULT_CAPACITY * object_size);
    *vec = (uint8_t*)data + sizeof(nu_vector_header_t);
    nu_vector_header_t *header = (nu_vector_header_t*)data;
    header->size        = 0;
    header->capacity    = VECTOR_DEFAULT_CAPACITY;
    header->object_size = object_size;
}
void nu_vector_free_(nu_vector_t vec)
{
    nu_free(nu_vector_get_header_(vec));
}
void nu_vector_clear(nu_vector_t vec)
{
    nu_vector_get_header_(vec)->size = 0;
}
bool nu_vector_is_empty(nu_vector_t vec)
{
    return nu_vector_get_header_(vec)->size == 0;
}
void *nu_vector_get_last_(nu_vector_t vec)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    if (header->size == 0) return NULL;
    return (uint8_t*)(vec) + header->object_size * (header->size - 1);
}
bool nu_vector_find_index(nu_vector_t vec, nu_vector_eq_pfn_t eq, const void *user, uint32_t *index)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    for (uint32_t i = 0; i < header->size; i++) {
        if (eq((uint8_t*)vec + header->object_size * i, user)) {
            *index = i;
            return true;
        }
    }
    return false;
}
uint32_t nu_vector_get_size(nu_vector_t vec)
{
    return nu_vector_get_header_(vec)->size;
}
uint32_t nu_vector_get_capacity(nu_vector_t vec)
{
    return nu_vector_get_header_(vec)->capacity;
}
uint32_t nu_vector_get_allocated_memory(nu_vector_t vec)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    return sizeof(nu_vector_header_t) + header->object_size * header->capacity;
}
void *nu_vector_push_(nu_vector_t *vec)
{
    nu_vector_header_t *header = nu_vector_get_header_(*vec);
    header->size++;
    if (header->size > header->capacity) {
        header->capacity *= 2;
        header = nu_realloc(header, sizeof(nu_vector_header_t) + header->capacity * header->object_size);
        *vec = (uint8_t*)header + sizeof(nu_vector_header_t);
    }
    return (uint8_t*)(*vec) + header->object_size * (header->size - 1);
}
bool nu_vector_pop(nu_vector_t vec)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    if (header->size) {
        header->size--;
        return true;
    }
    return false;
}
void nu_vector_resize(nu_vector_t *vec, uint32_t size)
{
    nu_vector_header_t *header = nu_vector_get_header_(*vec);
    header->size = size;
    if (header->size > header->capacity) {
        header->capacity = size;
        header = nu_realloc(header, sizeof(nu_vector_header_t) + header->object_size * header->capacity);
        *vec = nu_vector_get_data_(header);
    }
}
void nu_vector_swap(nu_vector_t vec, uint32_t first, uint32_t second)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    NU_ASSERT(first < header->size);
    NU_ASSERT(second < header->size);
    if (first != second) {
        uint8_t *dfirst  = (uint8_t*)vec + first * header->object_size;
        uint8_t *dsecond = (uint8_t*)vec + second * header->object_size;
        nu_vector_swap_block(dfirst, dsecond, header->object_size);
    }
}
void nu_vector_swap_last(nu_vector_t vec, uint32_t i)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    nu_vector_swap(vec, i, header->size - 1);
}
void nu_vector_swap_last_pop(nu_vector_t vec, uint32_t i)
{
    nu_vector_swap_last(vec, i);
    nu_vector_pop(vec);
}
void *nu_vector_insert_(nu_vector_t *vec, uint32_t i)
{
    nu_vector_push_(vec);
    nu_vector_header_t *header = nu_vector_get_header_(*vec);
    NU_ASSERT(i < header->size);
    if (i < header->size - 1) {
        size_t length_to_move = header->size - i - 1; 
        void *src = (uint8_t*)(*vec) + i * header->object_size; 
        void *dst = (uint8_t*)src + header->object_size;
        memmove(dst, src, header->object_size * length_to_move);
    }
    return (uint8_t*)(*vec) + i * header->object_size;
}
void nu_vector_erase(nu_vector_t vec, uint32_t i)
{
    nu_vector_header_t *header = nu_vector_get_header_(vec);
    NU_ASSERT(i < header->size);
    if (i != header->size - 1) {
        size_t length_to_move = header->size - i - 1; 
        void *src = (uint8_t*)vec + (i + 1) * header->object_size; 
        void *dst = (uint8_t*)src - header->object_size;
        memmove(dst, src, header->object_size * length_to_move);
    }
    nu_vector_pop(vec);
}

bool nu_vector_eq_int32(const void *a, const void *b)
{
    return *((int32_t*)a) == *((int32_t*)b);
}