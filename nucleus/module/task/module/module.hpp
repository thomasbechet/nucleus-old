#pragma once

#include <nucleus/nucleus.h>

extern "C"
{
    /* module */
    NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);
    NU_API nu_result_t nu_module_get_interface(const char *name, void *interface);
}