#ifndef NU_API_EVENT_H
#define NU_API_EVENT_H

#include <nucleus/core/utility/common.h>

NU_DECLARE_HANDLE(nu_event_t);

typedef nu_result_t (*nu_event_callback_pfn_t)(nu_event_t, void*);
typedef nu_result_t (*nu_event_initialize_pfn_t)(void*);
typedef nu_result_t (*nu_event_terminate_pfn_t)(void*);

typedef struct {
    size_t size;
    nu_event_initialize_pfn_t initialize;
    nu_event_terminate_pfn_t terminate;
} nu_event_info_t;

typedef struct {
    nu_event_t (*create)(const nu_event_info_t *info);
    void (*post)(nu_event_t event, void *data);
    void (*subscribe)(nu_event_t event, nu_event_callback_pfn_t callback);
    void (*dispatch)(nu_event_t event);
    void (*dispatch_all)(void);
} nu_event_api_t;

#endif