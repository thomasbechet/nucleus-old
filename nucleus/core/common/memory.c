#include "memory.h"

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "utility.h"
#include "logger.h"

#define NU_MEMORY_LOG_NAME "[MEMORY] "

static uint64_t nu_debug_memory_alloc_count = 0;
static uint64_t nu_debug_memory_free_count = 0;

void *nu_malloc(size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    nu_debug_memory_alloc_count++;
    return malloc(s);
#else
    return malloc(s);
#endif
}
void *nu_realloc(void *p, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    nu_debug_memory_alloc_count++;
    return realloc(p, s);
#else
    return realloc(p, s);
#endif
}
void *nu_calloc(size_t n, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    nu_debug_memory_alloc_count++;
    return calloc(n, s);
#else
    return calloc(n, s);
#endif
}
void nu_free(void *p)
{
#if defined(NU_DEBUG_MEMORY)
    if (nu_debug_memory_free_count >= nu_debug_memory_alloc_count) {
        nu_fatal(NU_MEMORY_LOG_NAME"Total alloc: %llu total free %llu]\n", nu_debug_memory_alloc_count, nu_debug_memory_free_count);
        nu_interrupt(NU_MEMORY_LOG_NAME"Free on non allocated memory detected. Exiting...\n");
    }
    nu_debug_memory_free_count++;
    free(p);
#else
    free(p);
#endif
}

uint64_t nu_memory_total_alloc(void)
{
    return nu_debug_memory_alloc_count;
}
uint64_t nu_memory_total_free(void)
{
    return nu_debug_memory_free_count;
}