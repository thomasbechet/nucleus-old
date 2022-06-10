#include <nucleus/core/system/allocator/linear.h>

#include <nucleus/core/system/allocator/memory.h>

void nu_allocator_linear_initialize(nu_allocator_linear_t *allocator, size_t max_size)
{
    allocator->data = nu_allocator_memory_commit(max_size);
    allocator->head = allocator->data;
    allocator->end  = allocator->data + max_size;
}
void nu_allocator_linear_terminate(nu_allocator_linear_t *allocator)
{
    nu_allocator_memory_uncommit(allocator->data);
}
void *nu_allocator_linear_malloc(nu_allocator_linear_t *allocator, size_t size, const char *file, uint32_t line)
{
    (void)file;
    (void)line;
    void *p = allocator->head;
    allocator->head += size;
    if (allocator->head >= allocator->end) {
        return NULL;
    }
    return p;
}
void nu_allocator_linear_reset(nu_allocator_linear_t *allocator)
{
    allocator->head = allocator->data;
}
void nu_allocator_linear_profile(nu_allocator_linear_t *allocator)
{
    // TODO:
    (void)allocator;
}
bool nu_allocator_linear_check_memory_leak(nu_allocator_linear_t *allocator)
{
    (void)allocator;
    return false;
}