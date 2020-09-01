#ifndef NU_EVENT_H
#define NU_EVENT_H

#include "../common/common.h"

typedef uint32_t nu_event_id_t;
typedef nu_result_t (*nu_event_callback_pfn_t)(nu_event_id_t, void*);

nu_result_t nu_event_initialize(void);
nu_result_t nu_event_terminate(void);
nu_result_t nu_event_dispatch_all(void);

NU_API nu_result_t nu_event_register(nu_event_id_t *id, uint32_t data_size);
NU_API nu_result_t nu_event_post(nu_event_id_t id, void *data);
NU_API nu_result_t nu_event_subscribe(nu_event_id_t id, nu_event_callback_pfn_t callback);

#endif