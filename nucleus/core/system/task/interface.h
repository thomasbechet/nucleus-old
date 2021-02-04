#ifndef NU_SYSTEM_TASK_INTERFACE_H
#define NU_SYSTEM_TASK_INTERFACE_H

#include "../../common/common.h"

/* task system interface */
#define NU_TASK_INTERFACE_LOADER_NAME "nu_task_get_interface"

NU_DECLARE_HANDLE(nu_task_handle_t);

typedef struct {
    void (*func)(void*, uint32_t, uint32_t);
    void *args;
} nu_task_job_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*create)(nu_task_handle_t *task);
    nu_result_t (*perform)(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count);
    nu_result_t (*wait)(nu_task_handle_t task);
    bool (*is_completed)(nu_task_handle_t task);
} nu_task_interface_t;

typedef nu_result_t (*nu_task_interface_loader_pfn_t)(nu_task_interface_t*);

#endif