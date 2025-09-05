#include <console/init.h>
#include <console/console.h>
#include <console/clear.h>

void console_init(void) {
    console_position = 0;

    console_clear_color(CONSOLE_COLOR(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_BLACK));
}