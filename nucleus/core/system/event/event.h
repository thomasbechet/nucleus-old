#ifndef NU_EVENT_H
#define NU_EVENT_H

#include <nucleus/core/api/event.h>

nu_result_t nu_event_initialize(void);
nu_result_t nu_event_terminate(void);
nu_result_t nu_event_register_api(void);

#endif