#include <console/print.h>
#include <console/inline_put.h>
#include <console/update.h>

#include <shell/blinker.h>

void console_print(const char * string) {
    clear_blinker();

    for (unsigned int i = 0; string[i] != '\0'; i++) {
        inline_put(string[i]);
    }

    console_update();

    update_blinker();
}