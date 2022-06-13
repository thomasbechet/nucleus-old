#ifndef NU_API_ENGINE_H
#define NU_API_ENGINE_H

#include <nucleus/core/utility/common.h>

typedef struct {
    void (*start)(void);
    void (*update)(void);
    void (*fixed_update)(void);
    void (*late_update)(void);
    void (*stop)(void);
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