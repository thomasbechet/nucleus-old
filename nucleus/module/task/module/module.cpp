#include <nucleus/module/task/module/module.hpp>

#include <nucleus/module/task/module/interface.h>
#include <nucleus/module/task/thread_pool.hpp>

static const uint32_t interface_count = 1;
static const char *interfaces[] = {
    NU_TASK_INTERFACE_NAME
};

static nucleus::thread_pool_t *_thread_pool;

static nu_result_t nutk_task_initialize(void)
{
    _thread_pool = new nucleus::thread_pool_t();
    _thread_pool->start(0);

    return NU_SUCCESS;
}
static nu_result_t nutk_task_terminate(void)
{
    _thread_pool->stop();
    delete _thread_pool;

    return NU_SUCCESS;
}
static nu_result_t nutk_task_start(void)
{
    return NU_SUCCESS;
}
static nu_result_t nutk_task_stop(void)
{
    return NU_SUCCESS;
}
static nu_result_t nutk_task_create(nu_task_handle_t *task)
{
    *task = (nu_task_handle_t)_thread_pool->create_task();
    return NU_SUCCESS;
}
static nu_result_t nutk_task_perform(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count)
{
    _thread_pool->perform(task, jobs, count);
    return NU_SUCCESS;
}
static nu_result_t nutk_task_wait(nu_task_handle_t task)
{
    _thread_pool->wait_task(task);
    return NU_SUCCESS;
}

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name            = NUTK_MODULE_NAME;
    info->id              = NUTK_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;
    info->plugin_count    = 0;
    info->plugins         = NULL;

    return NU_SUCCESS;
}
nu_result_t nu_module_get_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_TASK_INTERFACE_NAME)) {
        nu_task_interface_t *i = (nu_task_interface_t*)interface;

        i->initialize  = nutk_task_initialize;
        i->terminate   = nutk_task_terminate;
        i->start       = nutk_task_start;
        i->stop        = nutk_task_stop;
        i->create      = nutk_task_create;
        i->perform     = nutk_task_perform;
        i->wait        = nutk_task_wait;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}