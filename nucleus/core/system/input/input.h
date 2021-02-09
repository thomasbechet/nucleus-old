#ifndef NU_INPUT_H
#define NU_INPUT_H

#include "interface.h"

nu_result_t nu_input_initialize(void);
nu_result_t nu_input_terminate(void);
nu_result_t nu_input_start(void);
nu_result_t nu_input_stop(void);
nu_result_t nu_input_update(void);

#endif