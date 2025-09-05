#include <console/printn.h>
#include <console/inline_put.h>
#include <console/update.h>

#include <shell/blinker.h>

void console_printn(const char * str, uint32_t n) {
    clear_blinker();

    for (uint32_t i = 0; i < n; i++) {
        inline_put(str[i]);
    }

    update_blinker();

    console_update();
}