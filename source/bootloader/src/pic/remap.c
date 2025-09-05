#include <sys/ports.h>
#include <sys/asm/in.h>
#include <sys/asm/out.h>
#include <sys/asm/io_wait.h>

#include <pic/remap.h>

#define ICW1_ICW4       (0x01)
#define ICW1_SINGLE     (0x02)
#define ICW1_INTERVAL4  (0x04)
#define ICW1_LEVEL      (0x08)
#define ICW1_INIT       (0x10)

#define ICW4_8086       (0x01)
#define ICW4_AUTO       (0x02)
#define ICW4_BUF_SLAVE  (0x08)
#define ICW4_BUF_MASTER (0x0C)
#define ICW4_SFNM       (0x10)

void pic_remap(uint8_t master_offset, uint8_t slave_offset) {
    uint8_t master_mask, slave_mask;

    master_mask = inb(PORT_PIC1_DATA);
    slave_mask  = inb(PORT_PIC2_DATA);

    outb(PORT_PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PORT_PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PORT_PIC1_DATA, master_offset);
    io_wait();
    outb(PORT_PIC2_DATA, slave_offset);
    io_wait();
    outb(PORT_PIC1_DATA, 4);
    io_wait();
    outb(PORT_PIC2_DATA, 2);
    io_wait();

    outb(PORT_PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PORT_PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PORT_PIC1_DATA, master_mask);
    outb(PORT_PIC2_DATA, slave_mask);
}