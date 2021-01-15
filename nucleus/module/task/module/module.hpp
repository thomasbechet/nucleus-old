#pragma once

#include <nucleus/nucleus.h>

extern "C"
{
    /* module */
    NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);

    /* interface */
    NU_API nu_result_t nu_task_get_interface(nu_task_interface_t *interface);
}