#include <nucleus/core/event/event.h>

#include <nucleus/core/memory/memory.h>

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
} nu_event_data_t;

static nu_event_data_t _data;

nu_result_t nu_event_initialize(void)
{
    /* initialize data */
    _data.event_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_event_terminate(void)
{
    /* terminate all messages */
    for (uint32_t ei = 0; ei < _data.event_count; ei++) {
        for (uint32_t mi = 0; mi < _data.events[ei].message_count; mi++) {
            void *data = (void*)((char*)_data.events[ei].messages + mi * _data.events[ei].message_size);
            if (_data.events[ei].terminate) _data.events[ei].terminate(data);
        }
        _data.events[ei].message_count = 0;
    }

    /* free message buffers */
    for (uint32_t i = 0; i < _data.event_count; i++) {
        nu_free(_data.events[i].messages);
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
    for (uint32_t ei = 0; ei < _data.event_count; ei++) {
        for (uint32_t mi = 0; mi < _data.events[ei].message_count; mi++) {
            void *data = (void*)((char*)_data.events[ei].messages + mi * _data.events[ei].message_size);
            for (uint32_t si = 0; si < _data.events[ei].subscriber_count; si++) {
                _data.events[ei].subscribers[si](ei, data);
            }
            /* terminate message */
            if (_data.events[ei].terminate) _data.events[ei].terminate(data);
        }
        _data.events[ei].message_count = 0;
    }

    return NU_SUCCESS;
}

nu_result_t nu_event_register(nu_event_id_t *id, const nu_event_register_info_t *info)
{
    if (_data.event_count >= MAX_EVENT_COUNT) return NU_FAILURE;

    _data.events[_data.event_count].message_size  = info->size;
    _data.events[_data.event_count].message_count = 0;
    _data.events[_data.event_count].messages      = nu_malloc(info->size * MAX_MESSAGE_PER_EVENT_COUNT);
    _data.events[_data.event_count].initialize    = info->initialize;
    _data.events[_data.event_count].terminate     = info->terminate;

    *id = _data.event_count++;

    return NU_SUCCESS;
}
nu_result_t nu_event_post(nu_event_id_t id, void *data)
{
    void *dest = (void*)((char*)_data.events[id].messages + _data.events[id].message_count * _data.events[id].message_size);
    memcpy(dest, data, _data.events[id].message_size);
    _data.events[id].message_count++;

    /* initialize message */
    if (_data.events[id].initialize) _data.events[id].initialize(data);

    return NU_SUCCESS;
}
nu_result_t nu_event_subscribe(nu_event_id_t id, nu_event_callback_pfn_t callback)
{
    _data.events[id].subscribers[_data.events[id].subscriber_count] = callback;
    _data.events[id].subscriber_count++;

    return NU_SUCCESS;
}