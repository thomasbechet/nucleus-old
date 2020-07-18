#ifndef NU_SYSTEM_INPUT_H
#define NU_SYSTEM_INPUT_H

#include "../module/module.h"

typedef enum {
    NU_INPUT_API_NONE = 0,
    NU_INPUT_API_GLFW = 1
} nu_input_api_t;

/* private system functions */
nu_result_t nu_system_input_load(void);
nu_result_t nu_system_input_unload(void);
nu_result_t nu_system_input_update(void);

/* public system functions */
NU_API const nu_module_t *nu_system_input_get_module(void);

/* public input functions */
NU_API nu_result_t nu_input_is_button_pressed(void);

#endif