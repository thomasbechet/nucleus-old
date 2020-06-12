#ifndef NU_SYSTEM_TASK_H
#define NU_SYSTEM_TASK_H

#include "../module/module.h"

typedef struct {
    void (*func)(void*);
    void *args;
} nu_job_t;

typedef uint32_t nu_task_t;

/* private system functions */
nu_result_t nu_system_task_load(void);
nu_result_t nu_system_task_unload(void);

/* public system functions */
NU_API nu_module_t *nu_system_task_get_module(void);

/* public task functions */
NU_API nu_result_t nu_task_create(nu_task_t *task);
NU_API nu_result_t nu_task_add_job(nu_task_t task, nu_job_t job);
NU_API nu_result_t nu_task_add_jobs(nu_task_t task, nu_job_t *jobs, uint32_t count);
NU_API nu_result_t nu_task_perform(nu_task_t task);
NU_API nu_result_t nu_task_wait(nu_task_t task);
NU_API bool nu_task_is_completed(nu_task_t task);

#endif