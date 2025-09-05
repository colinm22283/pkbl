#include <console/inline_put.h>
#include <console/put.h>
#include <console/update.h>

#include <shell/blinker.h>

void console_put(char c) {
    clear_blinker();

    inline_put(c);

    update_blinker();

    console_update();
}