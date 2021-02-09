#ifndef NU_MODULE_H
#define NU_MODULE_H

#include "interface.h"

nu_result_t nu_module_initialize(void);
nu_result_t nu_module_terminate(void);
nu_result_t nu_module_start(void);
nu_result_t nu_module_stop(void);

#endif