#include <console/print_sdec.h>
#include <console/print_dec.h>
#include <console/update.h>
#include <console/put.h>

#include <shell/blinker.h>

void console_print_sdec(int32_t num) {
    clear_blinker();

    if (num < 0) {
        console_put('-');
        console_print_dec(-num);
    }
    else console_print_dec(num);

    update_blinker();

    console_update();
}