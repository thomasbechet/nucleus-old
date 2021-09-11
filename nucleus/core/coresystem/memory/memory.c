#include <nucleus/core/coresystem/memory/memory.h>

#include <nucleus/core/context/context.h>
#include <nucleus/core/coresystem/logger/logger.h>

typedef struct {
    uint64_t alloc_count;
    uint64_t alloc_aligned_count;
    uint64_t free_count;
    uint64_t free_aligned_count;
    uint64_t realloc_count;
    uint64_t realloc_aligned_count;
} nu_system_data_t;

static nu_system_data_t _system;

static void nu_memory_log_status(nu_severity_t severity)
{
    nu_core_log(severity, "[DEBUG MEMORY] [alloc:%llu free:%llu realloc:%llu]",
        _system.alloc_count, _system.free_count, _system.realloc_count);
    nu_core_log(severity, "[DEBUG MEMORY] [alloc-aligned:%llu free-aligned:%llu realloc-aligned:%llu]",
        _system.alloc_aligned_count, _system.free_aligned_count, _system.realloc_aligned_count);
}

nu_result_t nu_memory_initialize(void)
{
    _system.alloc_count           = 0;
    _system.alloc_aligned_count   = 0;
    _system.free_count            = 0;
    _system.free_aligned_count    = 0;
    _system.realloc_count         = 0;
    _system.realloc_aligned_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_memory_terminate(void)
{
#ifdef NU_DEBUG_MEMORY
    if (_system.alloc_count != _system.free_count) {
        nu_memory_log_status(NU_WARNING);
        nu_core_log(NU_WARNING, "[DEBUG MEMORY] Memory leak detected.");
    } else {
        nu_memory_log_status(NU_INFO);
        nu_core_log(NU_INFO, "[DEBUG MEMORY] No memory leak detected.");
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
#endif
    return malloc(s);
}
void *nu_malloc_aligned(size_t alignment, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _system.alloc_aligned_count++;
#endif

#if defined(NU_PLATFORM_WINDOWS)
    return _aligned_malloc(s, alignment);
#elif defined(NU_PLATFORM_UNIX)
    return aligned_alloc(alignment, s);
#endif
}
void *nu_realloc(void *p, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _system.realloc_count++;
#endif
    return realloc(p, s);
}
void *nu_realloc_aligned(void *p, size_t alignment, size_t s)
{
    (void)alignment;
#if defined(NU_DEBUG_MEMORY)
    _system.realloc_aligned_count++;
#endif

#if defined(NU_PLATFORM_WINDOWS)
    return _aligned_realloc(p, s, alignment);
#elif defined(NU_PLATFORM_UNIX)
    return realloc(p, s);
#endif
}
void *nu_calloc(size_t n, size_t s)
{
#if defined(NU_DEBUG_MEMORY)
    _system.alloc_count++;
#endif
    return calloc(n, s);
}
void nu_free(void *p)
{
#if defined(NU_DEBUG_MEMORY)
    if (_system.free_count >= _system.alloc_count) {
        nu_memory_log_status(NU_FATAL);
        nu_interrupt("[DEBUG MEMORY] Free on non allocated memory detected. Exiting...");
    }
    _system.free_count++;
#endif
    free(p);
}
void nu_free_aligned(void *p)
{
#if defined(NU_DEBUG_MEMORY)
    if (_system.free_aligned_count >= _system.alloc_aligned_count) {
        nu_memory_log_status(NU_FATAL);
        nu_interrupt("[DEBUG MEMORY] Free on non allocated aligned memory detected. Exiting...");
    }
    _system.free_aligned_count++;
#endif

#if defined(NU_PLATFORM_WINDOWS)
    _aligned_free(p);
#elif defined(NU_PLATFORM_UNIX)
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