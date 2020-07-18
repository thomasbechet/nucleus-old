#ifndef NU_CONTEXT_H
#define NU_CONTEXT_H

#include "config.h"
#include "../module/module.h"

typedef struct {
    nu_config_callback_t config_callback;
} nu_init_info_t;

NU_API nu_result_t nu_init(const nu_init_info_t *info);
NU_API nu_result_t nu_context_request_stop(void);
NU_API float nu_context_get_delta_time(void);

#endif