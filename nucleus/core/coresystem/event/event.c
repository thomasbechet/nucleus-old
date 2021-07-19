#include <nucleus/core/coresystem/event/event.h>

#include <nucleus/core/coresystem/memory/memory.h>

#define MAX_EVENT_COUNT 128
#define MAX_SUBSCRIBE_COUNT 128
#define MAX_MESSAGE_PER_EVENT_COUNT 64 

typedef struct {
    nu_event_callback_pfn_t subscribers[MAX_SUBSCRIBE_COUNT];
    uint32_t subscriber_count;
    void *messages;
    uint32_t message_count;
    uint32_t message_size;
    nu_event_initialize_pfn_t initialize;
    nu_event_terminate_pfn_t terminate;
} nu_event_buffer_t;

typedef struct {
    nu_event_buffer_t events[MAX_EVENT_COUNT];
    uint32_t event_count;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_event_initialize(void)
{
    /* initialize data */
    _system.event_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_event_terminate(void)
{
    /* terminate all messages */
    for (uint32_t ei = 0; ei < _system.event_count; ei++) {
        for (uint32_t mi = 0; mi < _system.events[ei].message_count; mi++) {
            void *data = (void*)((char*)_system.events[ei].messages + mi * _system.events[ei].message_size);
            if (_system.events[ei].terminate) _system.events[ei].terminate(data);
        }
        _system.events[ei].message_count = 0;
    }

    /* free message buffers */
    for (uint32_t i = 0; i < _system.event_count; i++) {
        nu_free(_system.events[i].messages);
    }

    return NU_SUCCESS;
}
nu_result_t nu_event_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_event_stop(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_event_dispatch_all(void)
{
    for (uint32_t ei = 0; ei < _system.event_count; ei++) {
        for (uint32_t mi = 0; mi < _system.events[ei].message_count; mi++) {
            void *data = (void*)((char*)_system.events[ei].messages + mi * _system.events[ei].message_size);
            for (uint32_t si = 0; si < _system.events[ei].subscriber_count; si++) {
                _system.events[ei].subscribers[si](ei, data);
            }
            /* terminate message */
            if (_system.events[ei].terminate) _system.events[ei].terminate(data);
        }
        _system.events[ei].message_count = 0;
    }

    return NU_SUCCESS;
}

nu_result_t nu_event_register(const nu_event_register_info_t *info, nu_event_id_t *id)
{
    if (_system.event_count >= MAX_EVENT_COUNT) return NU_FAILURE;

    _system.events[_system.event_count].message_size  = info->size;
    _system.events[_system.event_count].message_count = 0;
    _system.events[_system.event_count].messages      = nu_malloc(info->size * MAX_MESSAGE_PER_EVENT_COUNT);
    _system.events[_system.event_count].initialize    = info->initialize;
    _system.events[_system.event_count].terminate     = info->terminate;

    *id = _system.event_count++;

    return NU_SUCCESS;
}
nu_result_t nu_event_post(nu_event_id_t id, void *data)
{
    void *dest = (void*)((char*)_system.events[id].messages + _system.events[id].message_count * _system.events[id].message_size);
    memcpy(dest, data, _system.events[id].message_size);
    _system.events[id].message_count++;

    /* initialize message */
    if (_system.events[id].initialize) _system.events[id].initialize(data);

    return NU_SUCCESS;
}
nu_result_t nu_event_subscribe(nu_event_id_t id, nu_event_callback_pfn_t callback)
{
    _system.events[id].subscribers[_system.events[id].subscriber_count] = callback;
    _system.events[id].subscriber_count++;

    return NU_SUCCESS;
}