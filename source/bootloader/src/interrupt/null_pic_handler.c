#include <stdint.h>
#include <stdbool.h>

#include <defs.h>

#include <sys/ports.h>

#include <sys/asm/out.h>

void int_null_pic1_handler() {
    outb(PORT_PIC1_COMMAND, 0x20);
}

void int_null_pic2_handler() {
    outb(PORT_PIC1_COMMAND, 0x20);
    outb(PORT_PIC2_COMMAND, 0x20);
}