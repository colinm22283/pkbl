#include <console/console.h>
#include <console/newline.h>
#include <console/update.h>

#include <shell/blinker.h>

#include <memory/memcpy.h>

void console_newline(void) {
    clear_blinker();

    for (unsigned char i = 0; i < CONSOLE_HEIGHT - 1; i++) {
        memcpy(console_buffer[i], console_buffer[i + 1], CONSOLE_WIDTH * sizeof(console_char_t));
    }

    for (unsigned char i = 0; i < CONSOLE_WIDTH; i++) {
        console_buffer[CONSOLE_HEIGHT - 1][i].ch = '\0';
        console_buffer[CONSOLE_HEIGHT - 1][i].color = CONSOLE_COLOR(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_BLACK);
    }

    console_position = 0;

    update_blinker();

    console_update();
}