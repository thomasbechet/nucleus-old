#ifndef NU_EVENT_H
#define NU_EVENT_H

#include "interface.h"

nu_result_t nu_event_initialize(void);
nu_result_t nu_event_terminate(void);
nu_result_t nu_event_start(void);
nu_result_t nu_event_stop(void);
nu_result_t nu_event_dispatch_all(void);

#endif