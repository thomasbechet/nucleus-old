#include <nucleus/module/utils/console/console.hpp>

#include <nucleus/module/utils/console/logger.hpp>

#include <nucleus/module/utils/module/interface.h>

#include <memory>

#define FONT_SIZE 16

using namespace nu::utility;

typedef struct {
    std::unique_ptr<Console> console;
    nuutils_command_interface_t command_interface;
    bool command_interface_loaded;
} nu_console_data_t;

static nu_console_data_t _data;

static nu_result_t on_event(nu_event_id_t id, void *data)
{
    return _data.console->onEvent(id, data);
}

nu_result_t nuutils_console_plugin_initialize(void)
{
    /* load command interface */
    _data.command_interface_loaded = false;
    nu_result_t result;
    nu_module_handle_t module;
    result = nu_module_get_by_name(&module, NUUTILS_MODULE_NAME);
    if (result == NU_SUCCESS && nu_module_load_interface(module, NUUTILS_COMMAND_INTERFACE_NAME, &_data.command_interface) == NU_SUCCESS) {
        _data.command_interface_loaded = true;
    } else {
        nu_warning(NUUTILS_LOGGER_NAME"Using console without command plugin.\n");
    }

    // Subscribe event
    nu_event_subscribe(nu_renderer_viewport_resize_event_get_id(), on_event);

    // Create console
    _data.console = std::make_unique<Console>();
    
    return NU_SUCCESS;
}
nu_result_t nuutils_console_plugin_terminate(void)
{
    _data.console.reset();
    return NU_SUCCESS;
}
nu_result_t nuutils_console_plugin_update(void)
{
    _data.console->update();
    return NU_SUCCESS;
}

Console::Console()
{
    // Create font
    nu_renderer_font_create_info_t fontInfo;
    fontInfo.filename = "engine/font/Coder's Crux.ttf";
    fontInfo.font_size = FONT_SIZE;
    if (nu_renderer_font_create(&m_font, &fontInfo) != NU_SUCCESS) {
        nu_fatal(NUUTILS_LOGGER_NAME"Failed to create font.\n");
    }

    // Create cursor
    m_cursor = std::make_unique<Cursor>(500.0f);
    m_cursor->setVisible(false);
    m_selectedCharacter = 0;

    // Create command
    m_commandLine = std::make_unique<CommandLine>(m_font);
    m_commandLine->setVisible(false);
    m_selectedOldCommand = 0;

    // Create line buffer
    m_lineBuffer = std::make_unique<LineBuffer>(m_font, 15);

    // Create rectangle
    nu_renderer_rectangle_create_info_t info;
    info.color = 0xFFFFFF33;
    info.rect  = {0, 0, 0, 0};
    nu_renderer_rectangle_create(&m_rectangle, &info);

    // Set component position
    updatePosition();
    updateCursorAdvance();
}
Console::~Console()
{
    // Destroy linebuffer
    m_lineBuffer.reset();

    // Destroy command line
    m_commandLine.reset();

    // Destroy cursor
    m_cursor.reset();

    // Destroy font
    nu_renderer_font_destroy(m_font);
}

void Console::update()
{
    // Get cursor mode
    nu_cursor_mode_t cursorMode;
    nu_input_get_cursor_mode(&cursorMode);

    // Enable or disable console
    nu_button_state_t buttonState;
    nu_input_get_keyboard_state(&buttonState, NU_KEYBOARD_F1);
    if (buttonState & NU_BUTTON_JUST_PRESSED) {
        if (cursorMode == NU_CURSOR_MODE_NORMAL) {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_DISABLE);
            m_cursor->setVisible(false);
            m_commandLine->setVisible(false);
            m_lineBuffer->setVisible(false);
        } else {
            nu_input_set_cursor_mode(NU_CURSOR_MODE_NORMAL);
            m_cursor->setVisible(true);
            m_commandLine->setVisible(true);
            m_lineBuffer->setVisible(true);
        }
    }

    if (cursorMode == NU_CURSOR_MODE_NORMAL) {

        // append command line
        const char *str;
        uint32_t str_len;
        nu_input_get_keyboard_text(&str, &str_len);
        if (str_len) {
            std::string appendText(str);
            uint32_t commandSize = m_commandLine->size();
            m_commandLine->appendAt(m_selectedCharacter, appendText);
            if (m_selectedCharacter == commandSize) {
                m_selectedCharacter = m_commandLine->size();
            } else {
                m_selectedCharacter += appendText.size();
            }
            updateCursorAdvance();
        }

        // Backspace key
        nu_button_state_t backspaceState;
        nu_input_get_keyboard_state(&backspaceState, NU_KEYBOARD_BACKSPACE);
        if (backspaceState & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_commandLine->size() > 0) {
                if (m_selectedCharacter > 0) {
                    m_selectedCharacter--;
                    m_commandLine->removeAt(m_selectedCharacter);
                    updateCursorAdvance();
                }
            }
        }

        // arrow keys
        nu_button_state_t upState, downState, leftState, rightState;
        nu_input_get_keyboard_state(&upState, NU_KEYBOARD_UP);
        nu_input_get_keyboard_state(&downState, NU_KEYBOARD_DOWN);
        nu_input_get_keyboard_state(&leftState, NU_KEYBOARD_LEFT);
        nu_input_get_keyboard_state(&rightState, NU_KEYBOARD_RIGHT);
        if (upState & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selectedOldCommand > 0) {
                m_selectedOldCommand--;
                setCommandLine(m_oldCommands.at(m_selectedOldCommand));
            }
        } else if (downState & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selectedOldCommand < m_oldCommands.size()) {
                m_selectedOldCommand++;
                if (m_selectedOldCommand == m_oldCommands.size()) {
                    setCommandLine("");
                } else {
                    setCommandLine(m_oldCommands.at(m_selectedOldCommand));
                }
            } else {
                setCommandLine("");
            }
        } else if (leftState & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selectedCharacter > 0) {
                m_selectedCharacter--;
                updateCursorAdvance();
            }
        } else if (rightState & (NU_BUTTON_JUST_PRESSED | NU_BUTTON_REPEATED)) {
            if (m_selectedCharacter < m_commandLine->size()) {
                m_selectedCharacter++;
                updateCursorAdvance();
            }
        }

        /* enter key */
        nu_button_state_t enter_state;
        nu_input_get_keyboard_state(&enter_state, NU_KEYBOARD_ENTER);
        if (enter_state & NU_BUTTON_JUST_PRESSED) {
            if (m_commandLine->size() > 0) {
                // Add command to old commands
                m_oldCommands.push_back(m_commandLine->getCommand());
                m_selectedOldCommand = m_oldCommands.size();

                // Add command to the line buffer
                m_lineBuffer->add(m_commandLine->getCommand());

                // Execute command
                if (_data.command_interface_loaded) {
                    _data.command_interface.execute(m_commandLine->getCommand().c_str());
                }
                
                // Clear command line
                setCommandLine("");
            }
        }

        // Udpate cursor
        m_cursor->update(nu_context_get_delta_time());
    }
}
nu_result_t Console::onEvent(nu_event_id_t id, void *data)
{
    if (id == nu_renderer_viewport_resize_event_get_id()) {
        updatePosition();
    }

    return NU_SUCCESS;
}

void Console::updateCursorAdvance()
{
    std::string sub_string = m_commandLine->getText().substr(0, 1 + m_selectedCharacter);
    nu_vec2u_t size;
    nu_renderer_font_get_text_size(m_font, sub_string.c_str(), size);
    m_cursor->setAdvance(size[0]);
}
void Console::setCommandLine(std::string command)
{
    m_commandLine->setCommand(command);
    m_selectedCharacter = command.size();
    updateCursorAdvance();
}
void Console::updatePosition()
{
    Vector2u size;
    nu_renderer_viewport_get_size(size.data);

    // Get line height
    Vector2u lineSize;
    nu_renderer_font_get_text_size(m_font, "A", lineSize.data);
    lineSize.x = FONT_SIZE;
    
    // Cursor + Command Line
    m_commandLine->setPosition({(int32_t)lineSize.x, (int32_t)(size.y - lineSize.y * 2)});
    Rect rect = {(int32_t)lineSize.x - 1, (int32_t)(size.y - lineSize.y * 2) + 1, 200, lineSize.y};
    nu_renderer_rectangle_set_rect(m_rectangle, rect.data);
    m_cursor->setPosition({(int32_t)lineSize.x, (int32_t)(size.y - lineSize.y * 1)});

    // Line buffer
    m_lineBuffer->setPosition({(int32_t)lineSize.x, (int32_t)(size.y - lineSize.y * 3)});
}