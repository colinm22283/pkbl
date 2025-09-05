#include <console/set_color.h>
#include <console/update.h>
#include <console/console.h>

#include <shell/blinker.h>

void console_set_color(console_color_t color) {
    clear_blinker();

    for (unsigned char i = 0; i < CONSOLE_HEIGHT; i++) {
        for (unsigned char j = 0; j < CONSOLE_WIDTH; j++) {
            console_buffer[i][j].color = color;
        }
    }

    update_blinker();

    console_update();
}