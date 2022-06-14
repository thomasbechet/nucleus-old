#include <nucleus/core/system/engine/engine.h>

#include <nucleus/core/system/engine/api.h>
#include <nucleus/core/system/module/module.h>
#include <nucleus/core/system/module/api.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/api/input.h>
#include <nucleus/core/api/window.h>
#include <nucleus/core/api/renderer.h>

typedef struct {
    nu_input_system_api_t *input_api;
    nu_window_system_api_t *window_api;
    nu_renderer_system_api_t *renderer_api;
    nu_engine_callbacks_t callback;
    float delta_time;
    bool should_stop;
    nu_timer_t timer;
    bool enable_hotreload;
} nu_state_t;

static nu_state_t s_state;

static const nu_engine_api_t s_engine_api = {
    .run            = nu_engine_run,
    .request_stop   = nu_engine_request_stop,
    .get_delta_time = nu_engine_get_delta_time
};

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_engine_initialize(void)
{
    s_state.should_stop = false;
    return NU_SUCCESS;
}
nu_result_t nu_engine_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_engine_configure(bool enable_hotreload)
{
    s_state.enable_hotreload = enable_hotreload;
    return NU_SUCCESS;
}
nu_result_t nu_engine_register_api(void)
{
    return nu_module_register_api(nu_module_get_core(), nu_engine_api_t, &s_engine_api);
}

// +--------------------------------------------------------------------------+
// |                               PUBLIC API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_engine_run(const nu_engine_info_t *info)
{
    s_state.callback = info->callbacks;

    const float fixed_timestep   = 1.0f / 50.0f * 1000.0f;
    const float maximum_timestep = 1.0f / 10.0f * 1000.0f;
    float delta, accumulator;

    delta = accumulator = 0.0f;

    // Create timer
    s_state.timer = nu_timer_allocate(nu_allocator_get_core());
    nu_timer_start(s_state.timer);

    if (s_state.callback.start) {
        s_state.callback.start();
    }

    // Engine loop
    while (!s_state.should_stop) {

        // Auto hotreload
        if (s_state.enable_hotreload) {
            nu_module_hotreload_outdated();
        }
        
        // Compute delta
        delta = nu_timer_get_time_elapsed(s_state.timer);
        nu_timer_start(s_state.timer);

        if (delta > maximum_timestep)
            delta = maximum_timestep; // Slowing down

        accumulator += delta;

        // // Process window
        // NU_CHECK(nu_window_update() == NU_SUCCESS, goto cleanup0,
        //     nu_logger_get_core(), "Failed to update window system.");

        // // Process inputs
        // NU_CHECK(nu_input_update() == NU_SUCCESS, goto cleanup0,
        //     nu_logger_get_core(), "Failed to update input system.");

        // // Dispatch events
        // NU_CHECK(nu_event_dispatch_all() == NU_SUCCESS, goto cleanup0,
        //     nu_logger_get_core(), "Failed to dispatch events.");

        s_state.delta_time = fixed_timestep;
        while (accumulator >= fixed_timestep) {
            accumulator -= fixed_timestep;

            // Process fixed update
            if (s_state.callback.fixed_update) {
                s_state.callback.fixed_update();
            }
            
            // NU_CHECK(nu_plugin_fixed_update() == NU_SUCCESS, goto cleanup0,
            //     nu_logger_get_core(), "Failed to fixed update plugins.");
        }

        s_state.delta_time = delta;

        // // Process frame update
        // if (s_state.callback.update) {
        //     NU_CHECK(s_state.callback.update() == NU_SUCCESS, goto cleanup0,
        //         nu_logger_get_core(), "Failed to call 'update' application callback.");
        // }
        // NU_CHECK(nu_plugin_update() == NU_SUCCESS, goto cleanup0,
        //     nu_logger_get_core(), "Failed to update plugins.");

        // // Process late update
        // if (s_state.callback.late_update) {
        //     NU_CHECK(s_state.callback.late_update() == NU_SUCCESS, goto cleanup0,
        //         nu_logger_get_core(), "Failed to call 'late_update' application callback.");
        // }
        // NU_CHECK(nu_plugin_late_update() == NU_SUCCESS, goto cleanup0,
        //     nu_logger_get_core(), "Failed to late update plugins.");

        // // Process render
        // NU_CHECK(nu_renderer_render() == NU_SUCCESS, goto cleanup0,
        //     nu_logger_get_core(), "Failed to render renderer.");
    }

    if (s_state.callback.stop) {
        s_state.callback.stop();
    }

    // Free resources
// cleanup0:
    nu_timer_free(s_state.timer);

    return NU_SUCCESS;
}
void nu_engine_request_stop(void)
{
    s_state.should_stop = true;
}
float nu_engine_get_delta_time(void)
{
    return s_state.delta_time;
}