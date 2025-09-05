#include <console/clear.h>
#include <console/console.h>
#include <console/update.h>

#include <shell/blinker.h>

void console_clear(void) {
    clear_blinker();

    for (unsigned char i = 0; i < CONSOLE_HEIGHT; i++) {
        for (unsigned char j = 0; j < CONSOLE_WIDTH; j++) {
            console_buffer[i][j].ch = '\0';
        }
    }
    console_position = 0;

    update_blinker();

    console_update();
}