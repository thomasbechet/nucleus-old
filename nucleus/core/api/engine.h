#ifndef NU_API_ENGINE_H
#define NU_API_ENGINE_H

#include <nucleus/core/utility/result.h>

typedef struct {
    nu_result_t (*start)(void);
    nu_result_t (*update)(void);
    nu_result_t (*fixed_update)(void);
    nu_result_t (*late_update)(void);
    nu_result_t (*stop)(void);
} nu_engine_callbacks_t;

typedef struct {
    nu_engine_callbacks_t callbacks;
} nu_engine_info_t;

typedef struct {
    nu_result_t (*run)(const nu_engine_info_t *info);
    void (*request_stop)(void);
    float (*get_delta_time)(void);
} nu_engine_api_t;

#endif