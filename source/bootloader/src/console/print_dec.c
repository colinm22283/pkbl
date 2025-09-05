#include <console/print_dec.h>
#include <console/inline_put.h>
#include <console/update.h>
#include <console/put.h>

#include <shell/blinker.h>

void console_print_dec(uint32_t num) {
    clear_blinker();

    if (num == 0) {
        console_put('0');
    }
    else {
        char num_buf[10];

        uint8_t i;
        for (i = 0; num != 0; i++) {
            num_buf[i] = (char) ('0' + (num % 10));

            num /= 10;
        }

        while (i > 1 && num_buf[i - 1] == '0') {
            i--;
        }

        do {
            i--;

            inline_put(num_buf[i]);
        } while (i > 0);
    }

    update_blinker();

    console_update();
}