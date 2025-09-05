#include <shell/timer_handler.h>

#include <sys/asm/out.h>
#include <sys/ports.h>

void int_timer_handler() {
    shell_timer_handler();

    outb(PORT_PIC1_COMMAND, 0x20);
}