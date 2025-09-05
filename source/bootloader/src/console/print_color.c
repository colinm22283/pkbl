#include <console/print.h>
#include <console/inline_put.h>
#include <console/update.h>

#include <shell/blinker.h>

void console_print_color(const char * string, console_color_t color) {
    clear_blinker();

    for (unsigned int i = 0; string[i] != '\0'; i++) {
        inline_put_color(string[i], color);
    }

    console_update();

    update_blinker();
}