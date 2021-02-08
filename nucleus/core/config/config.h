#ifndef NU_CONFIG_H
#define NU_CONFIG_H

#include "interface.h"

nu_result_t nu_config_load(nu_config_callback_pfn_t callback);
nu_result_t nu_config_unload(void);

#endif