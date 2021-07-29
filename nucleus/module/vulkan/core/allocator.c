#include <nucleus/module/vulkan/core/allocator.h>

static void *allocation_function(void *user, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope)
{
    if (size == 0) return NULL;
    void *ptr = nu_malloc_aligned(alignment, size);
    memset(ptr, 0, size);
    return ptr;
}
static void free_function(void *user, void *memory)
{
    if (memory == NULL) return;
    nu_free_aligned(memory);
}
static void *reallocation_function(void *user, void *original, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope)
{
    if (original == NULL) return NULL;
    if (size == 0) {
        nu_free_aligned(original);
        return NULL;
    }
    return nu_realloc_aligned(original, alignment, size);
}
static void internal_allocation_function(void *user, size_t size, VkInternalAllocationType allocation_type, VkSystemAllocationScope allocation_scope)
{
    
}
static void internal_free_function(void *user, size_t size, VkInternalAllocationType allocation_type, VkSystemAllocationScope allocation_scope)
{
       
}

nu_result_t nuvk_allocator_initialize(VkAllocationCallbacks *allocator)
{
    memset(allocator, 0, sizeof(VkAllocationCallbacks));
    allocator->pUserData             = NULL;
    allocator->pfnAllocation         = allocation_function;
    allocator->pfnFree               = free_function;
    allocator->pfnReallocation       = reallocation_function;
    allocator->pfnInternalAllocation = internal_allocation_function;
    allocator->pfnInternalFree       = internal_free_function;

    return NU_SUCCESS;
}