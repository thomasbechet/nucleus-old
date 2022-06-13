#ifndef NU_ENGINE_H
#define NU_ENGINE_H

#include <nucleus/core/api/engine.h>

nu_result_t nu_engine_initialize(void);
nu_result_t nu_engine_terminate(void);
nu_result_t nu_engine_configure(bool enable_hotreload);
nu_result_t nu_engine_register_api(void);

#endif