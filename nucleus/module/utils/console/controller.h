#ifndef NUUTILS_CONSOLE_CONTROLLER_H
#define NUUTILS_CONSOLE_CONTROLLER_H

#include <nucleus/module/utils/console/view.h>
#include <nucleus/module/utils/module/interface.h>

typedef struct {
    nu_string_t command;
    bool visible;
    uint32_t selected_character;
} nuutils_console_controller_t;

void nuutils_console_controller_initialize(nuutils_console_controller_t *controller);
void nuutils_console_controller_terminate(nuutils_console_controller_t *controller);
void nuutils_console_controller_update(nuutils_console_controller_t *controller, nuutils_console_view_t *view,
                                       nuutils_command_interface_t *interface);

#endif