#include <disc/select.h>

#include <sys/asm/out.h>
#include <sys/asm/in.h>

bool disc_select(port_t _io_port, uint8_t drive_id) {
    const ata_pio_io_port_t * io_port = (const ata_pio_io_port_t *) (intptr_t) _io_port;

    outb_ptr(&io_port->drive_head, drive_id);
    outb_ptr(&io_port->lba_low, 0);
    outb_ptr(&io_port->lba_mid, 0);
    outb_ptr(&io_port->lba_high, 0);
    outb_ptr(&io_port->command, ATA_PIO_COMMAND_IDENTIFY);

    union { uint8_t uint8; ata_pio_status_t value; } status = { .uint8 = inb_ptr(&io_port->status), };
    for (int i = 0; i < 256; i++) {
//        while ((status.value.busy || !status.value.ready) && !status.value.error) status.uint8 = inb_ptr(&io_port->status);
//        if (status.value.error) {
//            return false;
//        }

        inw_ptr(&io_port->data);
    }

    return true;
}