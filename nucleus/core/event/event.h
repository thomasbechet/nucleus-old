#ifndef NU_EVENT_H
#define NU_EVENT_H

#include "../common/common.h"

typedef uint32_t nu_event_id_t;
typedef nu_result_t (*nu_event_callback_pfn_t)(nu_event_id_t, void*);
typedef nu_result_t (*nu_event_initialize_pfn_t)(void*);
typedef nu_result_t (*nu_event_terminate_pfn_t)(void*);

typedef struct {
    uint32_t size;
    nu_event_initialize_pfn_t initialize;
    nu_event_terminate_pfn_t terminate;
} nu_event_register_info_t;

nu_result_t nu_event_initialize(void);
nu_result_t nu_event_terminate(void);
nu_result_t nu_event_dispatch_all(void);

NU_API nu_result_t nu_event_register(nu_event_id_t *id, const nu_event_register_info_t *info);
NU_API nu_result_t nu_event_post(nu_event_id_t id, void *data);
NU_API nu_result_t nu_event_subscribe(nu_event_id_t id, nu_event_callback_pfn_t callback);

#endif