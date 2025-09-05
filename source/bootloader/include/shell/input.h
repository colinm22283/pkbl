#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <console/print.h>

#include <shell/history.h>

#define SHELL_INPUT_BUFFER_SIZE (256)

extern volatile uint8_t shell_position;
extern char shell_input_buffer[SHELL_INPUT_BUFFER_SIZE];
extern bool shell_ready_to_execute;
extern bool shell_enable_input_write_back;

static inline bool shell_input_char(char c) {
    if (shell_position == SHELL_INPUT_BUFFER_SIZE - 2) return false;

    shell_input_buffer[shell_position++] = c;

    return true;
}

static inline bool shell_backspace() {
    if (shell_position == 0) return false;

    shell_position--;

    return true;
}

static inline void shell_read_command() {
    shell_input_buffer[shell_position] = '\0';

    shell_history_push(shell_input_buffer);

    shell_input_buffer[shell_position + 1] = '\0';
    for (int i = 0; shell_input_buffer[i] != '\0'; i++) {
        if (shell_input_buffer[i] == ' ') {
            shell_input_buffer[i] = '\0';
        }
    }

    shell_position = 0;

    shell_ready_to_execute = true;
}