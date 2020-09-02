#include "module.hpp"

#include "interface.h"
#include "../thread_pool.hpp"

static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

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
    info->id = NUTK_MODULE_ID;
    info->flags = NU_MODULE_FLAG_TYPE_TASK;
    info->plugin_count = plugin_count;
    info->plugins = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_task_get_interface(nu_task_interface_t *interface)
{
    interface->initialize  = nutk_task_initialize;
    interface->terminate   = nutk_task_terminate;
    interface->create      = nutk_task_create;
    interface->perform     = nutk_task_perform;
    interface->wait        = nutk_task_wait;

    return NU_SUCCESS;
}