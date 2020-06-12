#ifndef NU_MODULE_INTERFACE_H
#define NU_MODULE_INTERFACE_H

#include "../common/common.h"
#include "../system/task.h"

#define NU_TASK_INTERFACE_NAME        "nu_task_interface"
#define NU_TASK_INTERFACE_LOADER_NAME "nu_task_interface_loader"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*create_task)(nu_task_t *task);
    nu_result_t (*add_job)(nu_task_t task, nu_job_t job);
    nu_result_t (*add_jobs)(nu_task_t task, nu_job_t *jobs, uint32_t count);
    nu_result_t (*perform)(nu_task_t task);
    nu_result_t (*wait)(nu_task_t task);
    bool (*is_completed)(nu_task_t task); 
} nu_task_interface_t;

typedef nu_result_t (*nu_task_interface_loader_pfn_t)(nu_task_interface_t*);

#define NU_WINDOW_INTERFACE_NAME        "nu_window_interface"
#define NU_WINDOW_INTERFACE_LOADER_NAME "nu_window_interface_loader"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_size)(uint32_t*, uint32_t*);
} nu_window_interface_t;

typedef nu_result_t (*nu_window_interface_loader_pfn_t)(nu_window_interface_t*);

#define NU_RENDERER_INTERFACE_NAME        "nu_renderer_interface"
#define NU_RENDERER_INTERFACE_LOADER_NAME "nu_renderer_interface_loader"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*render)(void);
} nu_renderer_interface_t;

typedef nu_result_t (*nu_renderer_interface_loader_pfn_t)(nu_renderer_interface_t*);

#endif