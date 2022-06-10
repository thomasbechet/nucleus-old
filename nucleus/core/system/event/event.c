#include <nucleus/core/system/event/event.h>

#include <nucleus/core/system/event/api.h>
#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/system/module/module.h>
#include <nucleus/core/system/module/api.h>

typedef struct {
    nu_vector(nu_event_callback_pfn_t) subscribers;
    nu_vector_t messages;
    size_t message_size;
    nu_event_initialize_pfn_t initialize;
    nu_event_terminate_pfn_t terminate;
} nu_event_data_t;

typedef struct {
    nu_vector(nu_event_data_t) events;
} nu_state_t;

static nu_state_t s_state;

static const nu_event_api_t s_event_api = {
    .create       = nu_event_create,
    .post         = nu_event_post,
    .subscribe    = nu_event_subscribe,
    .dispatch     = nu_event_dispatch,
    .dispatch_all = nu_event_dispatch_all
};

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_event_initialize(void)
{
    // Allocate resources
    s_state.events = nu_vector_allocate(nu_allocator_get_core(), nu_event_data_t);

    return NU_SUCCESS;
}
nu_result_t nu_event_terminate(void)
{
    // Terminate all messages
    for (nu_event_data_t *event_it = s_state.events; event_it != nu_vector_end(s_state.events); event_it++) {
        uint32_t message_count = nu_vector_size(event_it->messages);
        for (uint32_t mi = 0; mi < message_count; mi++) {
            if (event_it->terminate) {
                void *data = nu_vector_get_s(event_it->messages, event_it->message_size, mi);
                event_it->terminate(data);
            }
        }
        nu_vector_clear(event_it->messages);
    }

    // Free message buffers
    for (nu_event_data_t *event_it = s_state.events; event_it != nu_vector_end(s_state.events); event_it++) {
        nu_vector_free(event_it->messages);
        nu_vector_free(event_it->subscribers);
    }
    nu_vector_free(s_state.events);

    return NU_SUCCESS;
}
nu_result_t nu_event_register_api(void)
{
    return nu_module_register_api(nu_module_get_core(), nu_event_api_t, &s_event_api);
}

// +--------------------------------------------------------------------------+
// |                               PUBLIC API                                 |
// +--------------------------------------------------------------------------+

nu_event_t nu_event_create(const nu_event_info_t *info)
{
    // Create event
    nu_event_data_t event;
    event.subscribers   = nu_vector_allocate(nu_allocator_get_core(), nu_event_callback_pfn_t);
    event.initialize    = info->initialize;
    event.terminate     = info->terminate;
    event.messages      = nu_vector_allocate_s(nu_allocator_get_core(), info->size);
    event.message_size  = info->size;

    // Save the id
    nu_event_t handle; NU_HANDLE_SET_ID(handle, nu_vector_size(s_state.events));

    // Add the event
    nu_vector_push_value(&s_state.events, event); 

    return handle;
}
void nu_event_post(nu_event_t handle, void *data)
{
    // Add the message
    uint32_t id = NU_HANDLE_GET_ID(handle);

    nu_event_data_t *event = &s_state.events[id];
    void *p = nu_vector_push_s(&event->messages, event->message_size);
    memcpy(p, data, event->message_size);

    // Initialize message
    if (event->initialize) {
        event->initialize(data);
    }
}
void nu_event_subscribe(nu_event_t handle, nu_event_callback_pfn_t callback)
{
    uint32_t id = NU_HANDLE_GET_ID(handle);
    nu_event_data_t *event = &s_state.events[id];
    nu_vector_push_value(&event->subscribers, callback);
}
void nu_event_dispatch(nu_event_t handle)
{
    uint32_t id = NU_HANDLE_GET_ID(handle);
    nu_event_data_t *event = &s_state.events[id];

    // Dispatch
    uint32_t subscriber_count = nu_vector_size(event->subscribers);
    uint32_t message_count    = nu_vector_size(event->messages);
    for (uint32_t mi = 0; mi < message_count; mi++) {
        // Send message
        void *pmessage = nu_vector_get_s(event->messages, event->message_size, mi);
        for (uint32_t si = 0; si < subscriber_count; si++) {
            event->subscribers[si](handle, pmessage);
        }
        // Terminate message
        if (event->terminate) {
            event->terminate(pmessage);
        }
    }
    // Clear messages
    nu_vector_clear(event->messages);
}
void nu_event_dispatch_all(void)
{
    uint32_t event_count = nu_vector_size(s_state.events);
    for (uint32_t ei = 0; ei < event_count; ei++) {
        nu_event_t handle; NU_HANDLE_SET_ID(handle, ei); // Id are indices
        nu_event_dispatch(handle); 
    }
}