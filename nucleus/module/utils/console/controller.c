#include <nucleus/module/utils/console/controller.h>

void nuutils_console_controller_initialize(nuutils_console_controller_t *controller)
{
    nu_string_allocate(&controller->command);
    controller->visible = false;
    controller->selected_character = 0;
}
void nuutils_console_controller_terminate(nuutils_console_controller_t *controller)
{
    nu_string_free(controller->command);
}
void nuutils_console_controller_update(nuutils_console_controller_t *controller, nuutils_console_view_t *view,
                                       nuutils_command_interface_t *interface)
{
    /* get cursor mode */
    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);

    /* enable or disable console */
    nu_button_state_t button_state;
    nu_input_get_keyboard_state(NU_KEYBOARD_F1, &button_state);
    if (button_state & NU_BUTTON_JUST_PRESSED) {
        if (cursor_mode == NU_CURSOR_MODE_NORMAL) {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_DISABLE);
            nuutils_console_view_set_visible(view, false);
        } else {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_NORMAL);
            nuutils_console_view_set_visible(view, true);
        }
    }

    if (cursor_mode == NU_CURSOR_MODE_NORMAL) {

        /* append command line */
        const char *str;
        uint32_t str_len;
        nu_input_get_keyboard_text(&str, &str_len);
        if (str_len) {
            uint32_t command_size = nu_string_get_length(controller->command);
            nu_string_insert_cstr(&controller->command, str, controller->selected_character);
            nuutils_console_view_set_command(view, nu_string_get_cstr(controller->command));
            if (controller->selected_character == command_size) {
                controller->selected_character = nu_string_get_length(controller->command);
            } else {
                controller->selected_character += str_len;
            }
            nuutils_console_view_set_cursor_position(view, controller->selected_character);
        }

        /* backspace key */
        nu_button_state_t backspace_state;
        nu_input_get_keyboard_state(NU_KEYBOARD_BACKSPACE, &backspace_state);
        if (backspace_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (nu_string_get_length(controller->command) > 0) {
                if (controller->selected_character > 0) {
                    controller->selected_character--;
                    nu_string_erase(&controller->command, controller->selected_character, 1);
                    nuutils_console_view_set_cursor_position(view, controller->selected_character);
                }
                nuutils_console_view_set_command(view, nu_string_get_cstr(controller->command));
            }
        }

        /* arrow keys */
        nu_button_state_t up_state, down_state, left_state, right_state;
        nu_input_get_keyboard_state(NU_KEYBOARD_UP, &up_state);
        nu_input_get_keyboard_state(NU_KEYBOARD_DOWN, &down_state);
        nu_input_get_keyboard_state(NU_KEYBOARD_LEFT, &left_state);
        nu_input_get_keyboard_state(NU_KEYBOARD_RIGHT, &right_state);
        if (up_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            // if (console->selected_old_command > 0) {
            //     console->selected_old_command--;
            //     nu_string_t *old_command = (nu_string_t*)nu_array_get(console->old_commands, console->selected_old_command);
            //     nuutils_console_set_commandline(console, nu_string_get_cstr(*old_command));
            // }
        } else if (down_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            // if (console->selected_old_command < nu_array_get_size(console->old_commands)) {
            //     console->selected_old_command++;
            //     if (console->selected_old_command == nu_array_get_size(console->old_commands)) {
            //         nuutils_console_set_commandline(console, "");
            //     } else {
            //         nu_string_t *old_command = (nu_string_t*)nu_array_get(console->old_commands, console->selected_old_command);
            //         nuutils_console_set_commandline(console, nu_string_get_cstr(*old_command));
            //     }
            // } else {
            //     nuutils_console_set_commandline(console, "");
            // }
        } else if (left_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (controller->selected_character > 0) {
                controller->selected_character--;
                nuutils_console_view_set_cursor_position(view, controller->selected_character);
            }
        } else if (right_state & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (controller->selected_character < nu_string_get_length(controller->command)) {
                controller->selected_character++;
                nuutils_console_view_set_cursor_position(view, controller->selected_character);
            }
        }

        /* enter key */
        nu_button_state_t enter_state;
        nu_input_get_keyboard_state(NU_KEYBOARD_ENTER, &enter_state);
        if (enter_state & NU_BUTTON_JUST_PRESSED) {
            if (nu_string_get_length(controller->command) > 0) {
                /* add command to old commands */
                nuutils_console_view_add_old_command(view, nu_string_get_cstr(controller->command));

                /* execute command */
                if (interface) {
                    interface->execute(nu_string_get_cstr(controller->command));
                }
                
                /* clear command line */
                nuutils_console_view_set_command(view, "");
                nu_string_set_cstr(&controller->command, "");
            }
        }
    }

    nuutils_console_view_refresh(view);
}