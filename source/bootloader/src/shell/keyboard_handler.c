#include <shell/keyboard_handler.h>
#include <shell/input.h>
#include <shell/history.h>

#include <console/put.h>
#include <console/backspace.h>
#include <console/print.h>

#include <string/strlen.h>

volatile uint32_t history_position = -1;

void shell_keyboard_key_down_handler(keycode_t c) {
    if (!shell_ready_to_execute) {
        switch (c) {
            case 8: {
                if (shell_backspace()) console_backspace();
            } break;

            case '\n': {
                history_position = -1;
                shell_read_command();
            } break;

            case KC_DOWN_ARROW: {
                if (history_position != 0) {
                    for (uint8_t i = 0; i < shell_position; i++) console_backspace();

                    history_position--;

                    shell_history_get(history_position, shell_input_buffer);
                    shell_position = strlen(shell_input_buffer);
                    console_print(shell_input_buffer);
                }
                else {
                    for (uint8_t i = 0; i < shell_position; i++) console_backspace();
                    shell_position = 0;
                    history_position = -1;
                }
            } break;
            case KC_UP_ARROW: {
                if (history_position != SHELL_HISTORY_SIZE - 1) {
                    for (uint8_t i = 0; i < shell_position; i++) console_backspace();

                    history_position++;

                    shell_history_get(history_position, shell_input_buffer);
                    shell_position = strlen(shell_input_buffer);
                    console_print(shell_input_buffer);
                }
            } break;

            default: {
                if (c >= 32 && c <= 126 && shell_input_char((char) c)) {
                    if (shell_enable_input_write_back) console_put((char) c);
                }
            } break;
        }
    }
}