#include "memory.h"

#include "../context/context.h"
#include "../logger/logger.h"

#define NU_MEMORY_LOG_NAME "[MEMORY] "

typedef struct {
    uint64_t alloc_count;
    uint64_t free_count;
} nu_memory_data_t;

static nu_memory_data_t _memory;

nu_result_t nu_memory_initialize(void)
{
    _memory.alloc_count = 0;
    _memory.free_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_memory_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_memory_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_memory_stop(void)
{
    return NU_SUCCESS;
}

void *nu_malloc(size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _memory.alloc_count++;
    return malloc(s);
#else
    return malloc(s);
#endif
}
void *nu_realloc(void *p, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _memory.alloc_count++;
    return realloc(p, s);
#else
    return realloc(p, s);
#endif
}
void *nu_calloc(size_t n, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _memory.alloc_count++;
    return calloc(n, s);
#else
    return calloc(n, s);
#endif
}
void nu_free(void *p)
{
#if defined(NU_DEBUG_MEMORY)
    if (_memory.free_count >= _memory.alloc_count) {
        nu_fatal(NU_MEMORY_LOG_NAME"Total alloc: %llu total free %llu]\n", _memory.alloc_count, _memory.free_count);
        nu_interrupt(NU_MEMORY_LOG_NAME"Free on non allocated memory detected. Exiting...\n");
    }
    _memory.free_count++;
    free(p);
#else
    free(p);
#endif
}

uint64_t nu_memory_total_alloc(void)
{
    return _memory.alloc_count;
}
uint64_t nu_memory_total_free(void)
{
    return _memory.free_count;
}