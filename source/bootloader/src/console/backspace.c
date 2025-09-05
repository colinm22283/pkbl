#include <console/backspace.h>
#include <console/update.h>
#include <console/console.h>

#include <shell/blinker.h>

void console_backspace() {
    clear_blinker();

    console_position--;
    console_buffer[CONSOLE_HEIGHT - 1][console_position].ch = '\0';

    update_blinker();

    console_update();
}