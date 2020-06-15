#ifndef NU_SYSTEM_TASK_H
#define NU_SYSTEM_TASK_H

#include "../module/module.h"
#include "../module/interface.h"

/* private system functions */
nu_result_t nu_system_task_load(void);
nu_result_t nu_system_task_unload(void);

/* public system functions */
NU_API nu_module_t *nu_system_task_get_module(void);

/* public task functions */
NU_API nu_result_t nu_task_create(nu_task_t *task);
NU_API nu_result_t nu_task_perform(nu_task_t task, nu_job_t *jobs, uint32_t count);
NU_API nu_result_t nu_task_perform_parallel(nu_task_t task, nu_job_t, uint32_t count);
NU_API nu_result_t nu_task_wait(nu_task_t task);

#endif