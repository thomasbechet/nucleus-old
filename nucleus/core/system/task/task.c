#include <nucleus/core/system/task/task.h>

#include <nucleus/core/memory/interface.h>
#include <nucleus/core/logger/interface.h>

#include <nucleus/module/task.h>

#define NU_LOGGER_TASK_NAME "[TASK] "

typedef struct {
    nu_module_handle_t module;
    nu_task_interface_t interface;
} nu_task_data_t;

static nu_task_data_t _data;

nu_result_t nu_task_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_data, 0, sizeof(nu_task_data_t));

    /* get task module */
    result = nu_module_load(&_data.module, "engine/module/"NUTK_MODULE_NAME);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load task interface */
    result = nu_module_load_interface(_data.module, NU_TASK_INTERFACE_NAME, &_data.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_TASK_NAME"Failed to load interface.\n");
        return result;
    }

    /* initialize task system */
    result = _data.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_TASK_NAME"Failed to initialize task system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_task_terminate(void)
{
    return _data.interface.terminate();
}
nu_result_t nu_task_start(void)
{
    return _data.interface.start();
}
nu_result_t nu_task_stop(void)
{
    return _data.interface.stop();
}

nu_module_handle_t nu_task_get_module_handle(void)
{
    return _data.module;
}

nu_result_t nu_task_create(nu_task_handle_t *task)
{
    return _data.interface.create(task);
}
nu_result_t nu_task_perform(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count)
{
    return _data.interface.perform(task, jobs, count);
}
nu_result_t nu_task_perform_parallel(nu_task_handle_t task, nu_task_job_t job, uint32_t count)
{
    nu_result_t result;
    result = NU_SUCCESS;

    for (uint32_t i = 0; i < count; i++) {
        result = _data.interface.perform(task, &job, 1);
    }

    return result;
}
nu_result_t nu_task_wait(nu_task_handle_t task)
{
    return _data.interface.wait(task);
}