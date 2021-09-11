#include <nucleus/core/coresystem/event/event.h>

#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/utility/container/array.h>

typedef struct {
    nu_array_t subscribers;
    nu_array_t messages;
    nu_event_initialize_pfn_t initialize;
    nu_event_terminate_pfn_t terminate;
} nu_event_data_t;

typedef struct {
    nu_array_t events;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_event_initialize(void)
{
    /* allocate resources */
    nu_array_allocate(sizeof(nu_event_data_t), &_system.events);

    return NU_SUCCESS;
}
nu_result_t nu_event_terminate(void)
{
    /* terminate all messages */
    uint32_t event_count = nu_array_get_size(_system.events);
    nu_event_data_t *events = (nu_event_data_t*)nu_array_get_data(_system.events);
    for (uint32_t ei = 0; ei < event_count; ei++) {
        uint32_t message_count = nu_array_get_size(events[ei].messages);
        for (uint32_t mi = 0; mi < message_count; mi++) {
            if (events[ei].terminate) {
                events[ei].terminate(nu_array_get(events[ei].messages, mi));
            }
        }
        nu_array_clear(events[ei].messages);
    }

    /* free message buffers */
    for (uint32_t i = 0; i < event_count; i++) {
        nu_array_free(events[i].messages);
        nu_array_free(events[i].subscribers);
    }
    nu_array_free(_system.events);

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

nu_result_t nu_event_register(const nu_event_register_info_t *info, nu_event_id_t *id)
{
    /* create event */
    nu_event_data_t event;
    nu_array_allocate(info->size, &event.messages);
    nu_array_allocate(sizeof(nu_event_callback_pfn_t), &event.subscribers);
    event.initialize = info->initialize;
    event.terminate  = info->terminate;

    /* save the id */
    *id = nu_array_get_size(_system.events);

    /* add the event */
    nu_array_push(_system.events, &event);

    return NU_SUCCESS;
}
nu_result_t nu_event_post(nu_event_id_t id, void *data)
{
    /* add the message */
    nu_event_data_t *event = (nu_event_data_t*)nu_array_get(_system.events, id);
    nu_array_push(event->messages, data);

    /* initialize message */
    if (event->initialize) {
        event->initialize(data);
    }

    return NU_SUCCESS;
}
nu_result_t nu_event_subscribe(nu_event_id_t id, nu_event_callback_pfn_t callback)
{
    nu_event_data_t *event = (nu_event_data_t*)nu_array_get(_system.events, id);
    nu_array_push(event->subscribers, &callback);

    return NU_SUCCESS;
}
nu_result_t nu_event_dispatch(nu_event_id_t id)
{
    nu_event_data_t *event = (nu_event_data_t*)nu_array_get(_system.events, id);

    /* dispatch */
    uint32_t subscriber_count = nu_array_get_size(event->subscribers);
    uint32_t message_count = nu_array_get_size(event->messages);
    nu_event_callback_pfn_t *subscribers = (nu_event_callback_pfn_t*)nu_array_get_data(event->subscribers);
    for (uint32_t mi = 0; mi < message_count; mi++) {
        /* send message */
        void *data = nu_array_get(event->messages, mi);
        for (uint32_t si = 0; si < subscriber_count; si++) {
            subscribers[si](id, data);
        }
        /* terminate message */
        if (event->terminate) {
            event->terminate(data);
        }
    }
    /* clear messages */
    nu_array_clear(event->messages);

    return NU_SUCCESS;
}
nu_result_t nu_event_dispatch_all(void)
{
    uint32_t event_count = nu_array_get_size(_system.events);
    for (uint32_t ei = 0; ei < event_count; ei++) {
        nu_event_dispatch(ei); /* id are indexes */
    }

    return NU_SUCCESS;
}