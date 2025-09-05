#include <stdbool.h>

#include <defs.h>

#include <console/print.h>
#include <console/put.h>
#include <console/newline.h>
#include <console/print_hex.h>
#include <console/clear.h>
#include <console/print_dec.h>
#include <console/print_sdec.h>
#include <console/update.h>
#include <console/console.h>

#include <shell/input.h>
#include <shell/execute.h>
#include <shell/command_tree.h>
#include <shell/history.h>

#include <sys/wait_for_interrupt.h>

#include <memory/memcpy.h>

#include <memory_map.h>
#include <version.h>

#include <sys/halt.h>

#include <string/itoa.h>
#include <string/strcpy.h>

#include <disc/filesystem.h>
#include <boot/kernel.h>

#include <heap/alloc.h>
#include <heap/free.h>

#include <commands/ed.h>

void test();

__NORETURN void shell_entry(void) {
    shell_command_tree_init();
    shell_history_init();

    console_clear();
    console_print("PK Bootloader V" VERSION_STRING "\n");

    test();

    while (true) {
        console_print_color("PKBL> ", CONSOLE_COLOR(CONSOLE_COLOR_LIGHT_BLUE, CONSOLE_COLOR_BLACK));

        while (!shell_ready_to_execute) wait_for_interrupt();

        console_newline();
        uint32_t result = shell_execute(shell_input_buffer);
        if (result == SHELL_EXECUTE_COMMAND_NOT_FOUND) {
            console_print("Command not found \"");
            console_print(shell_input_buffer);
            console_print("\"\n");
        }
        else if (result > 0) {
            console_print("Command exited with code 0x");
            console_print_hex(result);
            console_newline();
        }


        shell_ready_to_execute = false;
    }
}