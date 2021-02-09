#ifndef NU_TASK_H
#define NU_TASK_H

#include "interface.h"

nu_result_t nu_task_initialize(void);
nu_result_t nu_task_terminate(void);
nu_result_t nu_task_start(void);
nu_result_t nu_task_stop(void);

#endif