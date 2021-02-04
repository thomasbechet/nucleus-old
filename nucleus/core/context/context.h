#ifndef NU_CONTEXT_H
#define NU_CONTEXT_H

#include "config.h"

typedef struct {
    nu_config_callback_pfn_t config;
    nu_result_t (*start)(void);
    nu_result_t (*update)(void);
    nu_result_t (*fixed_update)(void);
    nu_result_t (*late_update)(void);
    nu_result_t (*stop)(void);
} nu_context_callback_t;

typedef struct {
    nu_context_callback_t callback;
} nu_context_init_info_t;

NU_API nu_result_t nu_context_init(const nu_context_init_info_t *info);
NU_API nu_result_t nu_context_request_stop(void);
NU_API float nu_context_get_delta_time(void);
NU_API void nu_interrupt(const char *format, ...);

#endif