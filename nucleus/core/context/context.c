#include <nucleus/core/context/context.h>

#include <nucleus/core/config/config.h>
#include <nucleus/core/event/event.h>
#include <nucleus/core/logger/logger.h>
#include <nucleus/core/memory/memory.h>
#include <nucleus/core/module/module.h>
#include <nucleus/core/plugin/plugin.h>
#include <nucleus/core/system/input/input.h>
#include <nucleus/core/system/renderer/renderer.h>
#include <nucleus/core/system/task/task.h>
#include <nucleus/core/system/window/window.h>

#define NU_CONTEXT_LOGGER_NAME "[SYSTEM] "

typedef enum {
    NU_INIT_STATE_UNINITIALIZED,
    NU_INIT_STATE_INITIALIZED,
    NU_INIT_STATE_STARTED
} nu_init_state_t;

typedef struct {
    nu_init_state_t config;

    nu_init_state_t memory;
    nu_init_state_t logger;
    nu_init_state_t module;
    nu_init_state_t plugin;
    nu_init_state_t event;
    
    nu_init_state_t task;
    nu_init_state_t window;
    nu_init_state_t input;
    nu_init_state_t renderer;
} nu_system_states_t;

typedef struct {
    nu_context_callback_t callback;
    nu_system_states_t states;
    float delta_time;
    bool should_stop;
} nu_context_data_t;

static nu_context_data_t _data;

static nu_result_t nu_context_initialize(const nu_context_init_info_t *info);
static nu_result_t nu_context_terminate(void);

static nu_result_t nu_context_initialize(const nu_context_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_data, 0, sizeof(nu_context_data_t));
    _data.should_stop     = false;

    /* load configuration */
    if (info) _data.callback = info->callback;
    if (nu_config_load(_data.callback.config) != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE; }
    _data.states.config = NU_INIT_STATE_STARTED;

    /* initialize core systems */
    if (nu_memory_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.memory = NU_INIT_STATE_INITIALIZED;
    if (nu_logger_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.logger = NU_INIT_STATE_INITIALIZED;
    if (nu_module_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.module = NU_INIT_STATE_INITIALIZED;
    if (nu_plugin_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.plugin = NU_INIT_STATE_INITIALIZED;
    if (nu_event_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.event = NU_INIT_STATE_INITIALIZED;

    /* initialize systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Initializing systems...\n");
    if (nu_task_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.task = NU_INIT_STATE_INITIALIZED;
    if (nu_window_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.window = NU_INIT_STATE_INITIALIZED;
    if (nu_input_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.input = NU_INIT_STATE_INITIALIZED;
    if (nu_renderer_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.renderer = NU_INIT_STATE_INITIALIZED;

    /* start core systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting core systems...\n");
    if (nu_memory_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.memory = NU_INIT_STATE_STARTED;
    if (nu_logger_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.logger = NU_INIT_STATE_STARTED;
    if (nu_module_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.module = NU_INIT_STATE_STARTED;
    if (nu_plugin_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.plugin = NU_INIT_STATE_STARTED;
    if (nu_event_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.event = NU_INIT_STATE_STARTED;

    /* start systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting systems...\n");
    if (nu_task_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.task = NU_INIT_STATE_STARTED;
    if (nu_window_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.window = NU_INIT_STATE_STARTED;
    if (nu_input_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.input = NU_INIT_STATE_STARTED;
    if (nu_renderer_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _data.states.renderer = NU_INIT_STATE_STARTED;

    nu_info(NU_CONTEXT_LOGGER_NAME"Running context...\n");

    return result;
}
static nu_result_t nu_context_terminate(void)
{
    /* stop systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Stopping systems...\n");
    if (_data.states.renderer == NU_INIT_STATE_STARTED) {
        nu_renderer_stop();
        _data.states.renderer = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.input == NU_INIT_STATE_STARTED) {
        nu_input_stop();
        _data.states.input = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.window == NU_INIT_STATE_STARTED) {
        nu_window_stop();
        _data.states.window = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.task == NU_INIT_STATE_STARTED) {
        nu_task_stop();
        _data.states.task = NU_INIT_STATE_INITIALIZED;
    }

    /* stop core systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Stopping core systems...\n");
    if (_data.states.event == NU_INIT_STATE_STARTED) {
        nu_event_stop();
        _data.states.event = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.plugin == NU_INIT_STATE_STARTED) {
        nu_plugin_stop();
        _data.states.plugin = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.module == NU_INIT_STATE_STARTED) {
        nu_module_stop();
        _data.states.module = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.logger == NU_INIT_STATE_STARTED) {
        nu_logger_stop();
        _data.states.logger = NU_INIT_STATE_INITIALIZED;
    }
    if (_data.states.memory == NU_INIT_STATE_STARTED) {
        nu_memory_stop();
        _data.states.memory = NU_INIT_STATE_INITIALIZED;
    }

    /* terminate systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Terminating systems...\n");
    if (_data.states.renderer == NU_INIT_STATE_INITIALIZED) {
        nu_renderer_terminate();
        _data.states.renderer = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.input == NU_INIT_STATE_INITIALIZED) {
        nu_input_terminate();
        _data.states.input = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.window == NU_INIT_STATE_INITIALIZED) {
        nu_window_terminate();
        _data.states.window = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.task == NU_INIT_STATE_INITIALIZED) {
        nu_task_terminate();
        _data.states.task = NU_INIT_STATE_UNINITIALIZED;
    }

    /* terminate core systems */
    nu_info(NU_CONTEXT_LOGGER_NAME"Terminating core systems...\n");
    if (_data.states.event == NU_INIT_STATE_INITIALIZED) {
        nu_event_terminate();
        _data.states.event = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.plugin == NU_INIT_STATE_INITIALIZED) {
        nu_plugin_terminate();
        _data.states.plugin = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.module == NU_INIT_STATE_INITIALIZED) {
        nu_module_terminate();
        _data.states.module = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.logger == NU_INIT_STATE_INITIALIZED) {
        nu_logger_terminate();
        _data.states.logger = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_data.states.memory == NU_INIT_STATE_INITIALIZED) {
        nu_memory_terminate();
        _data.states.memory = NU_INIT_STATE_UNINITIALIZED;
    }

    /* unload configuration */
    if (_data.states.config == NU_INIT_STATE_INITIALIZED) {
        nu_config_unload();
        _data.states.config = NU_INIT_STATE_UNINITIALIZED;
    }

    return NU_SUCCESS;
}

static nu_result_t nu_context_run(void)
{
    const float FIXED_TIMESTEP = 1.0f / 50.0f * 1000.0f;
    const float MAXIMUM_TIMESTEP = 1.0f / 10.0f * 1000.0f;
    float delta, accumulator;

    delta = accumulator = 0.0f;

    nu_timer_t timer;
    nu_timer_start(&timer);

    if (_data.callback.start)
        _data.callback.start();

    nu_module_log();

    while (!_data.should_stop) {
        /* compute delta */
        delta = nu_timer_get_time_elapsed(&timer);
        nu_timer_start(&timer);

        if (delta > MAXIMUM_TIMESTEP)
            delta = MAXIMUM_TIMESTEP; /* slowing down */

        accumulator += delta;

        /* process window */
        nu_window_update();

        /* process inputs */
        nu_input_update();

        /* dispatch events */
        nu_event_dispatch_all();

        _data.delta_time = FIXED_TIMESTEP;
        while (accumulator >= FIXED_TIMESTEP) {
            accumulator -= FIXED_TIMESTEP;

            /* process fixed update */
            if (_data.callback.fixed_update) 
                _data.callback.fixed_update();
            nu_plugin_fixed_update();
        }

        _data.delta_time = delta;

        /* process frame update */
        if (_data.callback.update) 
            _data.callback.update();
        nu_plugin_update();

        /* process late update */
        if (_data.callback.late_update) 
            _data.callback.late_update();
        nu_plugin_late_update();

        /* process render */
        nu_renderer_render();
    }

    if (_data.callback.stop) 
        _data.callback.stop();

    return NU_SUCCESS;
}

nu_result_t nu_context_init(const nu_context_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* start engine */
    result = nu_context_initialize(info);
    if (result != NU_SUCCESS) {
        nu_fatal(NU_CONTEXT_LOGGER_NAME"Failed to initialize nucleus engine.\n");
        return result;
    }
    result = nu_context_run();
    if (result != NU_SUCCESS) {
        nu_warning(NU_CONTEXT_LOGGER_NAME"Nucleus engine didn't end correctly.\n");
    }
    result = nu_context_terminate();
    if (result != NU_SUCCESS) {
        nu_warning(NU_CONTEXT_LOGGER_NAME"Nucleus engine didn't terminate correctly.\n");
    }

    /* memory check */
#ifdef NU_DEBUG
    if (nu_memory_total_alloc() != nu_memory_total_free()) {
        nu_warning(NU_CONTEXT_LOGGER_NAME"Memory leak detected (alloc: %llu free: %llu).\n", nu_memory_total_alloc(), nu_memory_total_free());
    } else {
        nu_info(NU_CONTEXT_LOGGER_NAME"No memory leak detected (alloc: %llu free: %llu).\n", nu_memory_total_alloc(), nu_memory_total_free());
    }
#endif

    nu_info(NU_CONTEXT_LOGGER_NAME"End process reached.\n");

    return NU_SUCCESS;
}
nu_result_t nu_context_request_stop(void)
{
    _data.should_stop = true;
    return NU_SUCCESS;
}
float nu_context_get_delta_time(void)
{
    return _data.delta_time;
}
void nu_interrupt(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_FATAL, format, args);
    va_end(args);
    exit(NU_FAILURE);
}