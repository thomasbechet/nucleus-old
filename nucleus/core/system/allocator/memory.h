#ifndef NU_ALLOCATOR_MEMORY_H
#define NU_ALLOCATOR_MEMORY_H

#include <nucleus/core/utility/platform.h>

void *nu_allocator_memory_commit(size_t size);
void nu_allocator_memory_uncommit(void *p);

#endif