#ifndef NU_SYSTEM_TASK_H
#define NU_SYSTEM_TASK_H

#include "../module/module.h"
#include "../module/interface.h"

/* private system functions */
nu_result_t nu_system_task_initialize(void);
nu_result_t nu_system_task_terminate(void);

/* public system functions */
NU_API nu_module_handle_t nu_system_task_get_module_handle(void);

/* public task functions */
NU_API nu_result_t nu_task_create(nu_task_handle_t *task);
NU_API nu_result_t nu_task_perform(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count);
NU_API nu_result_t nu_task_perform_parallel(nu_task_handle_t task, nu_task_job_t job, uint32_t count);
NU_API nu_result_t nu_task_wait(nu_task_handle_t task);

#endif