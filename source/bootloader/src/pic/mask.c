#include <stdint.h>

#include <sys/ports.h>
#include <sys/asm/in.h>
#include <sys/asm/out.h>

void pic_set_mask(unsigned char irq_line) {
    uint16_t port;
    uint8_t value;

    if (irq_line < 8) {
        port = PORT_PIC1_DATA;
    } else {
        port = PORT_PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void pic_clear_mask(unsigned char irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PORT_PIC1_DATA;
    } else {
        port = PORT_PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

void pic_mask_all() {
    outb(PORT_PIC1_DATA, 0xFF);
    outb(PORT_PIC2_DATA, 0xFF);
}