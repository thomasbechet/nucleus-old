#ifndef NU_MODULE_H
#define NU_MODULE_H

#include <nucleus/core/api/module.h>

nu_result_t nu_module_initialize(void);
nu_result_t nu_module_terminate(void);
nu_module_t nu_module_get_core(void);
nu_result_t nu_module_hotreload_outdated(void);

#endif