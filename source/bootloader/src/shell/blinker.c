#include <stdbool.h>

#include <console/console.h>
#include <console/update.h>

#include <shell/input.h>

bool blinker = false;
unsigned char last_location;

void clear_blinker() {
    if (!blinker) {
        console_buffer[CONSOLE_HEIGHT - 1][last_location].ch = ' ';
        blinker = true;
    }
}

void update_blinker() {
    if (!shell_ready_to_execute) {
        if (blinker) {
            last_location = console_position;
            console_buffer[CONSOLE_HEIGHT - 1][last_location].ch = (char) 0xDB;
        }
        else console_buffer[CONSOLE_HEIGHT - 1][last_location].ch = ' ';

        blinker = !blinker;

        console_update();
    }
};