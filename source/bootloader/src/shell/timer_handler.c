#include <stdbool.h>

#include <shell/timer_handler.h>
#include <shell/input.h>
#include <shell/blinker.h>

uint8_t timer_counter = 0;

void shell_timer_handler() {
    if (timer_counter == 6) {
        update_blinker();

        timer_counter = 0;
    }
    timer_counter++;
}