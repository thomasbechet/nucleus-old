#ifndef NU_API_TASK_H
#define NU_API_TASK_H

#include <nucleus/core/api/module.h>

NU_DECLARE_HANDLE(nu_task_t);

typedef enum {
    NU_TASK_MODULE_NONE = 0
} nu_task_module_t;

typedef struct {
    void (*func)(void*, uint32_t, uint32_t);
    void *args;
} nu_task_job_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*start)(void);
    nu_result_t (*stop)(void);
} nu_task_system_api_t;

typedef struct {
    nu_task_t (*create)(void);
    nu_result_t (*perform)(nu_task_t task, nu_task_job_t *jobs, uint32_t count);
    nu_result_t (*wait)(nu_task_t task);
    bool (*is_completed)(nu_task_t task);
} nu_task_api_t;

NU_API nu_module_t nu_task_get_module(void);

NU_API nu_task_t nu_task_create(void);
NU_API nu_result_t nu_task_perform(nu_task_t task, nu_task_job_t *jobs, uint32_t count);
NU_API nu_result_t nu_task_perform_parallel(nu_task_t task, nu_task_job_t job, uint32_t count);
NU_API nu_result_t nu_task_wait(nu_task_t task);

#endif