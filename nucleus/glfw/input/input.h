#ifndef NUGLFW_INPUT_H
#define NUGLFW_INPUT_H

#include "../../core/nucleus_core.h"

/* window interface */
nu_result_t nuglfw_input_initialize(void);
nu_result_t nuglfw_input_terminate(void);
nu_result_t nuglfw_input_update(void);

#endif