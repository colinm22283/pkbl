#include <stdint.h>
#include <stdbool.h>

#include <shell/execute.h>
#include <shell/command_tree.h>
#include <shell/input.h>

#include <console/print.h>
#include <console/newline.h>
#include <console/print_dec.h>

static inline bool is_valid_cmd_char(char c) {
    return c >= 'a' && c < 'z';
}

uint32_t shell_execute(char * command) {
    if (command[0] == '\0') return 0;

    if (!is_valid_cmd_char(command[0])) return SHELL_EXECUTE_COMMAND_NOT_FOUND;
    const shell_command_tree_entry_t * current_entry = shell_command_tree_root[(uint8_t) command[0] - 'a'];

    for (uint32_t i = 1; command[i] != '\0'; i++) {
        if (!is_valid_cmd_char(command[i])) return SHELL_EXECUTE_COMMAND_NOT_FOUND;

        current_entry = current_entry->next_char[(uint8_t) command[i] - 'a'];

        if (current_entry == NULL) return SHELL_EXECUTE_COMMAND_NOT_FOUND;
    }

    if (current_entry->command_ptr == NULL) return SHELL_EXECUTE_COMMAND_NOT_FOUND;

    uint32_t argc = 1;
    const char * argv[SHELL_MAX_ARGS] = { command };

    for (uint32_t i = 0; true; i++) {
        if (i >= SHELL_INPUT_BUFFER_SIZE - 1) return SHELL_EXECUTE_COMMAND_NOT_FOUND;

        if (shell_input_buffer[i] == '\0' && shell_input_buffer[i + 1] == '\0') break;

        if (command[i] == '\0') {
            argv[argc++] = &shell_input_buffer[i + 1];
        }
    }

    return current_entry->command_ptr(argc, argv);
}