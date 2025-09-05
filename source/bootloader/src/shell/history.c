#include <shell/history.h>
#include <shell/input.h>

#include <string/strcpy.h>

uint32_t current_position = SHELL_HISTORY_SIZE - 1;
char history[SHELL_HISTORY_SIZE][SHELL_INPUT_BUFFER_SIZE];

void shell_history_init() {
    for (uint32_t i = 0; i < SHELL_HISTORY_SIZE; i++) history[i][0] = '\0';
}

void shell_history_push(const char * command) {
    if (current_position == 0) current_position = SHELL_HISTORY_SIZE - 1;
    else current_position--;

    strcpy(history[current_position], command);
}

void shell_history_get(uint32_t index, char * dest) {
    uint32_t actual_index = (current_position + index) % SHELL_HISTORY_SIZE;

    strcpy(dest, history[actual_index]);
}