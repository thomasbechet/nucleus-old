#include <nucleus/core/coresystem/memory/memory.h>

#include <nucleus/core/context/context.h>
#include <nucleus/core/coresystem/logger/logger.h>

typedef struct {
    uint64_t alloc_count;
    uint64_t free_count;
    uint64_t realloc_count;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_memory_initialize(void)
{
    _system.alloc_count   = 0;
    _system.free_count    = 0;
    _system.realloc_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_memory_terminate(void)
{
#ifdef NU_DEBUG_MEMORY
    if (_system.alloc_count != _system.free_count) {
        nu_core_log(NU_WARNING, "[DEBUG MEMORY] Memory leak detected (alloc: %llu free: %llu realloc: %llu).\n", 
            _system.alloc_count, _system.free_count, _system.realloc_count);
    } else {
        nu_core_log(NU_INFO, "[DEBUG MEMORY] No memory leak detected (alloc: %llu free: %llu realloc: %llu).\n", 
            _system.alloc_count, _system.free_count, _system.realloc_count);
    }
#endif

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
    _system.alloc_count++;
    return malloc(s);
#else
    return malloc(s);
#endif
}
void *nu_malloc_aligned(size_t alignment, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _system.alloc_count++;
    return aligned_alloc(alignment, s);
#else
    return aligned_alloc(alignment, s);
#endif
}
void *nu_realloc(void *p, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _system.realloc_count++;
    return realloc(p, s);
#else
    return realloc(p, s);
#endif
}
void *nu_realloc_aligned(void *p, size_t alignment, size_t s)
{
    (void)alignment;
#if defined(NU_DEBUG_MEMORY)
    _system.realloc_count++;
    return realloc(p, s);
#else
    return realloc(p, s);
#endif
}
void *nu_calloc(size_t n, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _system.alloc_count++;
    return calloc(n, s);
#else
    return calloc(n, s);
#endif
}
void nu_free(void *p)
{
#if defined(NU_DEBUG_MEMORY)
    if (_system.free_count >= _system.alloc_count) {
        nu_core_log(NU_FATAL, "[DEBUG MEMORY] Total alloc: %llu total free %llu total realloc %llu]\n", 
            _system.alloc_count, _system.free_count, _system.realloc_count);
        nu_interrupt("[DEBUG MEMORY] Free on non allocated memory detected. Exiting...\n");
    }
    _system.free_count++;
    free(p);
#else
    free(p);
#endif
}

uint64_t nu_memory_total_alloc(void)
{
    return _system.alloc_count;
}
uint64_t nu_memory_total_free(void)
{
    return _system.free_count;
}
uint64_t nu_memory_total_realloc(void)
{
    return _system.realloc_count;
}