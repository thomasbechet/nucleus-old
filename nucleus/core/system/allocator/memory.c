#include <nucleus/core/system/allocator/memory.h>

void *nu_allocator_memory_commit(size_t size)
{
    return malloc(size);
}
void nu_allocator_memory_uncommit(void *p)
{
    free(p);
}