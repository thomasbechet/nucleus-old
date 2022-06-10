#ifndef NU_ALLOCATOR_LINEAR_H
#define NU_ALLOCATOR_LINEAR_H

#include <nucleus/core/utility/platform.h>

typedef struct {
    uint8_t *data;
    uint8_t *end;
    uint8_t *head;
} nu_allocator_linear_t;

void nu_allocator_linear_initialize(nu_allocator_linear_t *allocator, size_t max_size);
void nu_allocator_linear_terminate(nu_allocator_linear_t *allocator);
void *nu_allocator_linear_malloc(nu_allocator_linear_t *allocator, size_t size, const char *file, uint32_t line);
void nu_allocator_linear_reset(nu_allocator_linear_t *allocator);
void nu_allocator_linear_profile(nu_allocator_linear_t *allocator);
bool nu_allocator_linear_check_memory_leak(nu_allocator_linear_t *allocator);

#endif