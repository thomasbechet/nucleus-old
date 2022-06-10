#ifndef NU_EVENT_API_H
#define NU_EVENT_API_H

#include <nucleus/core/api/event.h>

NU_API nu_event_t nu_event_create(const nu_event_info_t *info);
NU_API void nu_event_post(nu_event_t event, void *data);
NU_API void nu_event_subscribe(nu_event_t event, nu_event_callback_pfn_t callback);
NU_API void nu_event_dispatch(nu_event_t event);
NU_API void nu_event_dispatch_all(void);

#endif