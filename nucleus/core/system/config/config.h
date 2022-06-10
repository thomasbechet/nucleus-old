#ifndef NU_CONFIG_H
#define NU_CONFIG_H

#include <nucleus/core/api/config.h>

nu_result_t nu_config_initialize(void);
nu_result_t nu_config_terminate(void);
nu_result_t nu_config_register_api(void);

#endif