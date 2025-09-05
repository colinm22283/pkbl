#include <console/clear.h>
#include <console/console.h>
#include <console/update.h>

#include <shell/blinker.h>

void console_clear_color(console_color_t color) {
    clear_blinker();

    for (unsigned char i = 0; i < CONSOLE_HEIGHT; i++) {
        for (unsigned char j = 0; j < CONSOLE_WIDTH; j++) {
            console_buffer[i][j].ch = '\0';
            console_buffer[i][j].color = color;
        }
    }
    console_position = 0;

    update_blinker();

    console_update();
}