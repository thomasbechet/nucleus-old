#ifndef NU_ALLOCATOR_FREELIST_H
#define NU_ALLOCATOR_FREELIST_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/type.h>

typedef enum {
    NU_BLOCK_FLAG_USED      = 1,
    NU_BLOCK_FLAG_ALLOCATED = 2
} nu_block_flags_t;

struct nu_block {
    size_t size;
    struct nu_block *next;
    struct nu_block *prev;
    uint8_t flags;
#ifdef NU_DEBUG_MEMORY
    uint32_t line;
    const char *file;
#endif
    nu_word_t data[1];
};

typedef struct nu_block nu_block_t;

typedef struct {
    nu_block_t *first;
    nu_block_t *last;
} nu_allocator_freelist_t;

void nu_allocator_freelist_initialize(nu_allocator_freelist_t *allocator);
void nu_allocator_freelist_terminate(nu_allocator_freelist_t *allocator);
void *nu_allocator_freelist_malloc(nu_allocator_freelist_t *allocator, size_t size, const char *file, uint32_t line);
void *nu_allocator_freelist_realloc(nu_allocator_freelist_t *allocator, void *p, size_t s, const char *file, uint32_t line);
void nu_allocator_freelist_free(nu_allocator_freelist_t *allocator, void *p, const char *file, uint32_t line);
void nu_allocator_freelist_profile(nu_allocator_freelist_t *allocator);
bool nu_allocator_freelist_check_memory_leak(nu_allocator_freelist_t *allocator);

#endif