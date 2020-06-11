#ifndef NU_CONFIG_H
#define NU_CONFIG_H

#include "../system/renderer.h"
#include "../system/window.h"

typedef struct {
    nu_window_api_t window_api;
    nu_renderer_api_t renderer_api;
} nu_config_t;

typedef nu_result_t (*nu_config_callback_t)(nu_config_t*);

nu_result_t nu_config_default(nu_config_t *config);

#endif