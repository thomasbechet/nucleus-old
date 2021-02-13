#ifndef NU_MEMORY_H
#define NU_MEMORY_H

#include <nucleus/core/memory/interface.h>

nu_result_t nu_memory_initialize(void);
nu_result_t nu_memory_terminate(void);
nu_result_t nu_memory_start(void);
nu_result_t nu_memory_stop(void);

#endif