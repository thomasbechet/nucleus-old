#ifndef NU_TASK_INTERFACE_H
#define NU_TASK_INTERFACE_H

#include <nucleus/core/coresystem/module/interface.h>

#define NU_TASK_INTERFACE_NAME "nu_task_interface"

NU_DECLARE_HANDLE(nu_task_handle_t);

typedef struct {
    void (*func)(void*, uint32_t, uint32_t);
    void *args;
} nu_task_job_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*start)(void);
    nu_result_t (*stop)(void);
    nu_result_t (*create)(nu_task_handle_t *task);
    nu_result_t (*perform)(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count);
    nu_result_t (*wait)(nu_task_handle_t task);
    bool (*is_completed)(nu_task_handle_t task);
} nu_task_interface_t;

NU_API nu_module_handle_t nu_task_get_module_handle(void);

NU_API nu_result_t nu_task_create(nu_task_handle_t *task);
NU_API nu_result_t nu_task_perform(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count);
NU_API nu_result_t nu_task_perform_parallel(nu_task_handle_t task, nu_task_job_t job, uint32_t count);
NU_API nu_result_t nu_task_wait(nu_task_handle_t task);

#endif