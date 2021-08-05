#include <nucleus/core/system/task/task.h>

#include <nucleus/core/config/interface.h>
#include <nucleus/core/coresystem/memory/interface.h>
#include <nucleus/core/coresystem/logger/interface.h>

#define NU_LOGGER_TASK_NAME "[TASK] "

typedef struct {
    nu_module_t module;
    nu_task_interface_t interface;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_task_initialize(void)
{
    nu_result_t result;

    memset(&_system, 0, sizeof(nu_system_data_t));
    nu_task_api_t api = nu_config_get().task.api;

    if (api != NU_TASK_API_NONE) {
        /* get task module */
        result = nu_module_load("engine/module/none", &_system.module);
        if (result != NU_SUCCESS) {
            return result;
        }

        /* get task interface */
        result = nu_module_get_interface(_system.module, NU_TASK_INTERFACE_NAME, &_system.interface);
        if (result != NU_SUCCESS) {
            nu_error(NU_LOGGER_TASK_NAME"Failed to get interface.\n");
            return result;
        }

        /* initialize task system */
        if (_system.interface.initialize) {
            result = _system.interface.initialize();
            if (result != NU_SUCCESS) {
                nu_error(NU_LOGGER_TASK_NAME"Failed to initialize task system.\n");
                return result;
            }
        }
    } else {
        nu_info(NU_LOGGER_TASK_NAME"Running in passive mode.\n");
    }

    return NU_SUCCESS;
}
nu_result_t nu_task_terminate(void)
{
    if (_system.interface.terminate) {
        return _system.interface.terminate();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_task_start(void)
{
    if (_system.interface.start) {
        return _system.interface.start();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_task_stop(void)
{
    if (_system.interface.stop) {
        return _system.interface.stop();
    } else {
        return NU_SUCCESS;
    }
}

nu_module_t nu_task_get_module(void)
{
    return _system.module;
}

nu_result_t nu_task_create(nu_task_t *task)
{
    if (_system.interface.create) {
        return _system.interface.create(task);
    } else {
        *task = NU_NULL_HANDLE;
        return NU_SUCCESS;
    }
}
nu_result_t nu_task_perform(nu_task_t task, nu_task_job_t *jobs, uint32_t count)
{ 
    if (_system.interface.perform) {
        return _system.interface.perform(task, jobs, count);
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_task_perform_parallel(nu_task_t task, nu_task_job_t job, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        nu_result_t result = nu_task_perform(task, &job, 1);
        if (result != NU_SUCCESS) {
            nu_warning(NU_LOGGER_TASK_NAME"Failed to perform task.");
        }
    }

    return NU_SUCCESS;
}
nu_result_t nu_task_wait(nu_task_t task)
{
    if (_system.interface.wait) {
        return _system.interface.wait(task);
    } else {
        return NU_SUCCESS;
    }
}