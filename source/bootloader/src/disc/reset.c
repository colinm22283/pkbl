#include <disc/reset.h>

#include <sys/asm/in.h>
#include <sys/asm/out.h>

void disc_reset(port_t _control_port) {
    const ata_pio_control_port_t * control_port = (const ata_pio_control_port_t *) (intptr_t) _control_port;

    outb_ptr(&control_port->device_control, ATA_PIO_DEVICE_CONTROL_SOFTWARE_RESET);
    outb_ptr(&control_port->device_control, 0);

    inb_ptr(&control_port->status);
    inb_ptr(&control_port->status);
    inb_ptr(&control_port->status);
    inb_ptr(&control_port->status);

    union { uint8_t uint8; ata_pio_status_t value; } status = { .uint8 = inb(0x1F7), };
    while ((status.value.busy || !status.value.ready) && !status.value.error) status.uint8 = inb(0x1F7);
}