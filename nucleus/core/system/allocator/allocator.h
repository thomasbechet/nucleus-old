#ifndef NU_ALLOCATOR_H
#define NU_ALLOCATOR_H

#include <nucleus/core/api/allocator.h>
#include <nucleus/core/utility/common.h>

nu_result_t nu_allocator_initialize(void);
nu_result_t nu_allocator_terminate(void);
nu_result_t nu_allocator_register_api(void);
nu_result_t nu_allocator_check_memory_leak(void);
nu_allocator_t nu_allocator_get_core(void);

#endif