#ifndef NU_ENGINE_API_H
#define NU_ENGINE_API_H

#include <nucleus/core/api/engine.h>
#include <nucleus/core/utility/common.h>

NU_API nu_result_t nu_engine_run(const nu_engine_info_t *info);
NU_API void nu_engine_request_stop(void);
NU_API float nu_engine_get_delta_time(void);

#endif