#include <nucleus/core/system/allocator/allocator.h>

#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/system/allocator/freelist.h>
#include <nucleus/core/system/allocator/linear.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/system/module/module.h>
#include <nucleus/core/system/module/api.h>

#include <cJSON/cJSON.h>

#define MAX_NAME_LENGTH 64

typedef uint16_t nu_offset_t;

typedef enum {
    NU_ALLOCATOR_TYPE_FREELIST,
    NU_ALLOCATOR_TYPE_LINEAR,
    NU_ALLOCATOR_TYPE_STACK
} nu_allocator_type_t;

typedef struct {
    nu_allocator_type_t type;
    char name[MAX_NAME_LENGTH];
    union {
        nu_allocator_freelist_t freelist;
        nu_allocator_linear_t linear;
    };
} nu_allocator_data_t;

typedef struct {
    nu_vector(nu_allocator_data_t) allocators;
    nu_allocator_t core_allocator;
} nu_state_t;

static nu_state_t s_state;

static nu_allocator_api_t s_allocator_api = {
    .malloc         = nu_malloc_,
    .malloc_aligned = nu_malloc_aligned_,
    .calloc         = nu_calloc_,
    .free           = nu_free_,
    .free_aligned   = nu_free_aligned_
};

static void *nu_json_malloc_hook(size_t s) 
{
    return nu_malloc(nu_allocator_get_core(), s);
}
static void nu_json_free_hook(void *p)
{
    nu_free(nu_allocator_get_core(), p);
}

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_allocator_initialize(void)
{
    // Initialize core allocator
    nu_allocator_data_t core_allocator;
    core_allocator.type = NU_ALLOCATOR_TYPE_FREELIST;
    nu_snprintf(core_allocator.name, MAX_NAME_LENGTH, "core");
    nu_allocator_freelist_initialize(&core_allocator.freelist);
    
    // Assign the core allocator to the fake first slot 
    s_state.allocators = &core_allocator;
    NU_HANDLE_SET_ID(s_state.core_allocator, 0);
    // Allocate the allocator vector with one capacity to ensure the next push operation
    // will not require a reallocation
    s_state.allocators = nu_vector_allocate_capacity(nu_allocator_get_core(), nu_allocator_data_t, 1);
    // Insert the core allocator
    nu_vector_push_value(&s_state.allocators, core_allocator);

    // Setup cJSON hooks
    cJSON_Hooks hooks;
    hooks.malloc_fn = nu_json_malloc_hook;
    hooks.free_fn   = nu_json_free_hook;
    cJSON_InitHooks(&hooks);

    return NU_SUCCESS;
}
nu_result_t nu_allocator_terminate(void)
{
    // Terminate allocators
    for (nu_allocator_data_t *it = s_state.allocators + 1; it != nu_vector_end(s_state.allocators); it++) {
        switch (it->type) {
            case NU_ALLOCATOR_TYPE_FREELIST:
                nu_allocator_freelist_terminate(&it->freelist);
                break;
            case NU_ALLOCATOR_TYPE_LINEAR:
                nu_allocator_linear_terminate(&it->linear);
                break;
            case NU_ALLOCATOR_TYPE_STACK:
                break;
        }
    }

    // Save core allocator
    nu_allocator_data_t core_allocator = s_state.allocators[0];

    // Free resources from core allocator
    nu_vector_free(s_state.allocators);

    // Terminate core allocator
    NU_ASSERT(!nu_allocator_freelist_check_memory_leak(&core_allocator.freelist));
    nu_allocator_freelist_terminate(&core_allocator.freelist);

    return NU_SUCCESS;
}
nu_result_t nu_allocator_register_api(void)
{
    return nu_module_register_api(nu_module_get_core(), nu_allocator_api_t, &s_allocator_api);
}
nu_result_t nu_allocator_check_memory_leak(void)
{
    // Skip the first allocator which is the core allocator
    for (nu_allocator_data_t *it = s_state.allocators + 1; it != nu_vector_end(s_state.allocators); it++) {
        // Check memory leak
        bool leak = false;
        switch (it->type)
        {
        case NU_ALLOCATOR_TYPE_FREELIST:
            leak = nu_allocator_freelist_check_memory_leak(&it->freelist);
            break;
        case NU_ALLOCATOR_TYPE_LINEAR:
            leak = nu_allocator_linear_check_memory_leak(&it->linear);
            break;
        case NU_ALLOCATOR_TYPE_STACK:
            break;
        }
        // Warn the user
        if (leak) {
            nu_warning(nu_logger_get_core(), "Memory leak detected for allocator '%s'.", it->name);
        }
    }
    return NU_SUCCESS;
}
nu_allocator_t nu_allocator_get_core(void)
{
    return s_state.core_allocator;
}

// +--------------------------------------------------------------------------+
// |                               PUBLIC API                                 |
// +--------------------------------------------------------------------------+

nu_allocator_t nu_allocator_create_freelist(const char *name)
{
    nu_allocator_t handle; NU_HANDLE_SET_ID(handle, nu_vector_size(s_state.allocators));
    nu_allocator_data_t *allocator = nu_vector_push(&s_state.allocators);
    allocator->type = NU_ALLOCATOR_TYPE_FREELIST;
    nu_allocator_freelist_initialize(&allocator->freelist);
    strncpy(allocator->name, name, MAX_NAME_LENGTH);
    return handle;
}
nu_allocator_t nu_allocator_create_linear(const char *name, size_t max_size)
{
    nu_allocator_t handle; NU_HANDLE_SET_ID(handle, nu_vector_size(s_state.allocators));
    nu_allocator_data_t *allocator = nu_vector_push(&s_state.allocators);
    allocator->type = NU_ALLOCATOR_TYPE_LINEAR;
    nu_allocator_linear_initialize(&allocator->linear, max_size);
    strncpy(allocator->name, name, MAX_NAME_LENGTH);
    return handle;
}
nu_allocator_t nu_allocator_create_stack(const char *name)
{
    (void)name;
    return NU_NULL_HANDLE;
}
void nu_allocator_destroy(nu_allocator_t allocator)
{
    nu_allocator_data_t *data = &s_state.allocators[NU_HANDLE_GET_ID(allocator)];
    switch (data->type)
    {
    case NU_ALLOCATOR_TYPE_FREELIST:
        nu_allocator_freelist_terminate(&data->freelist);
        break;
    case NU_ALLOCATOR_TYPE_LINEAR:
        nu_allocator_linear_terminate(&data->linear);
        break;
    case NU_ALLOCATOR_TYPE_STACK:
        // nu_allocator_stack_terminate(&data->stack);
        break;
    }
}
void nu_allocator_reset(nu_allocator_t allocator)
{
    nu_allocator_data_t *data = &s_state.allocators[NU_HANDLE_GET_ID(allocator)];
    switch (data->type)
    {
    case NU_ALLOCATOR_TYPE_FREELIST:
        nu_warning(nu_logger_get_core(), "Unsupported reset operation for freelist allocator.");
        break;
    case NU_ALLOCATOR_TYPE_LINEAR:
        nu_allocator_linear_reset(&data->linear);
        break;
    case NU_ALLOCATOR_TYPE_STACK:
        break;
    }
}
void *nu_malloc_(nu_allocator_t allocator, size_t size, const char *file, uint32_t line)
{
    nu_allocator_data_t *data = &s_state.allocators[NU_HANDLE_GET_ID(allocator)];
    switch (data->type)
    {
    case NU_ALLOCATOR_TYPE_FREELIST:
        return nu_allocator_freelist_malloc(&data->freelist, size, file, line);
    case NU_ALLOCATOR_TYPE_LINEAR:
        return nu_allocator_linear_malloc(&data->linear, size, file, line);
    case NU_ALLOCATOR_TYPE_STACK:
        return NULL;
    default:
        return NULL;
    }
}
void *nu_malloc_aligned_(nu_allocator_t allocator, size_t size, size_t alignment, const char *file, uint32_t line)
{
    size_t offset = alignment - 1;
    // Allocate
    void *p = nu_malloc_(allocator, size + sizeof(nu_offset_t) + (alignment - 1), file, line);
    if (p == NULL) return NULL;
    // Shift the pointer
    void *aligned = (void*)(((size_t)p + offset) & ~(offset));
    // Store offset
    *((size_t*)aligned - 1) = (size_t)aligned - (size_t)p;
    // Return aligned pointer
    return aligned;
}
void *nu_realloc_(nu_allocator_t allocator, void *p, size_t s, const char *file, uint32_t line)
{
    nu_allocator_data_t *data = &s_state.allocators[NU_HANDLE_GET_ID(allocator)];
    switch (data->type)
    {
    case NU_ALLOCATOR_TYPE_FREELIST:
        return nu_allocator_freelist_realloc(&data->freelist, p, s, file, line);
    case NU_ALLOCATOR_TYPE_LINEAR:
        nu_warning(nu_logger_get_core(), "Unsupported realloc operation for linear allocator.");
        return NULL;
    case NU_ALLOCATOR_TYPE_STACK:
        return NULL;
    default:
        return NULL;
    }
}
void *nu_realloc_aligned_(nu_allocator_t allocator, void *aligned, size_t alignment, size_t s, const char *file, uint32_t line)
{
    // Get the old pointer
    size_t shift = *((size_t*)aligned - 1);
    void *p = (void*)((size_t)aligned - shift);
    // Compute required offset
    size_t offset = alignment - 1;
    // Reallocate
    p = nu_realloc_(allocator, p, s + sizeof(nu_offset_t) + (alignment - 1), file, line);
    if (p == NULL) return NULL;
    // Shift the pointer
    aligned = (void*)(((size_t)p + offset) & ~(offset));
    // Store offset
    *((size_t*)aligned - 1) = (size_t)aligned - (size_t)p;
    // Return aligned pointer
    return aligned;
}
void *nu_calloc_(nu_allocator_t allocator, size_t n, size_t s, const char *file, uint32_t line)
{
    void *p = nu_malloc_(allocator, n * s, file, line);
    memset(p, 0, n * s);
    return p;
}
void nu_free_(nu_allocator_t allocator, void *p, const char *file, uint32_t line)
{
    nu_allocator_data_t *data = &s_state.allocators[NU_HANDLE_GET_ID(allocator)];
    switch (data->type)
    {
    case NU_ALLOCATOR_TYPE_FREELIST:
        nu_allocator_freelist_free(&data->freelist, p, file, line);
        break;
    case NU_ALLOCATOR_TYPE_LINEAR:
        nu_warning(nu_logger_get_core(), "Unsupported operation for linear allocator: %s %u.", file, line);
        break;
    case NU_ALLOCATOR_TYPE_STACK:
        break;
    }
}
void nu_free_aligned_(nu_allocator_t allocator, void *p, const char *file, uint32_t line)
{
    size_t offset = *((size_t*)p - 1);
    nu_free_(allocator, (void*)((size_t)p - offset), file, line);
}