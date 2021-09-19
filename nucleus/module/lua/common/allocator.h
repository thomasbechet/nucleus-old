#ifndef NULUA_ALLOCATOR_H
#define NULUA_ALLOCATOR_H

#include <nucleus/nucleus.h>

void *nulua_allocator(void *ud, void *ptr, size_t osize, size_t nsize);

#endif