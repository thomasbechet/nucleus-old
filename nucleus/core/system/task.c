#include "task.h"

#include "../module/interface.h"
#include "../../task/module/interface.h"

#define NU_TASK_LOG_NAME "[TASK] "

typedef struct {
    nu_module_t module;
    nu_task_interface_t interface;
} nu_system_task_t;

static nu_system_task_t _system;

nu_result_t nu_system_task_load(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_system, 0, sizeof(nu_system_task_t));

    /* load task module */
    result = nu_module_load(&_system.module, NUTK_MODULE_NAME);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load task interface accessor */
    nu_task_interface_loader_pfn_t load_interface;
    result = nu_module_load_function(&_system.module, NU_TASK_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_TASK_LOG_NAME" Failed to load task loader.\n");
        return result;
    }

    /* load task interface */
    result = load_interface(&_system.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_TASK_LOG_NAME" Failed to load interface.\n");
        return result;
    }

    /* initialize task system */
    result = _system.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_TASK_LOG_NAME"Failed to initialize task system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_system_task_unload(void)
{
    /* terminate task system */
    _system.interface.terminate();

    /* unload module */
    nu_module_unload(&_system.module);
    memset(&_system.interface, 0, sizeof(nu_renderer_interface_t));

    return NU_SUCCESS;
}

nu_module_t *nu_system_task_get_module(void)
{
    return &_system.module;
}

nu_result_t nu_task_create(nu_task_t *task)
{
    return _system.interface.create(task);
}
nu_result_t nu_task_perform(nu_task_t task, nu_job_t *jobs, uint32_t count)
{
    return _system.interface.perform(task, jobs, count);
}
nu_result_t nu_task_perform_parallel(nu_task_t task, nu_job_t job, uint32_t count)
{
    nu_result_t result;
    result = NU_SUCCESS;

    for (uint32_t i = 0; i < count; i++) {
        result = _system.interface.perform(task, &job, 1);
    }

    return result;
}
nu_result_t nu_task_wait(nu_task_t task)
{
    return _system.interface.wait(task);
}