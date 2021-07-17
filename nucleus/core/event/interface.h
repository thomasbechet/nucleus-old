#ifndef NU_EVENT_INTERFACE_H
#define NU_EVENT_INTERFACE_H

#include <nucleus/core/common/common.h>

typedef uint32_t nu_event_id_t;
typedef nu_result_t (*nu_event_callback_pfn_t)(nu_event_id_t, void*);
typedef nu_result_t (*nu_event_initialize_pfn_t)(void*);
typedef nu_result_t (*nu_event_terminate_pfn_t)(void*);

typedef struct {
    uint32_t size;
    nu_event_initialize_pfn_t initialize;
    nu_event_terminate_pfn_t terminate;
} nu_event_register_info_t;

NU_API nu_result_t nu_event_register(const nu_event_register_info_t *info, nu_event_id_t *id);
NU_API nu_result_t nu_event_post(nu_event_id_t id, void *data);
NU_API nu_result_t nu_event_subscribe(nu_event_id_t id, nu_event_callback_pfn_t callback);

#endif