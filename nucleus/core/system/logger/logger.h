#ifndef NU_LOGGER_H
#define NU_LOGGER_H

#include <nucleus/core/api/logger.h>
#include <nucleus/core/utility/common.h>

nu_result_t nu_logger_initialize(void);
nu_result_t nu_logger_terminate(void);
nu_result_t nu_logger_register_api(void);
nu_logger_t nu_logger_get_core(void);

#endif