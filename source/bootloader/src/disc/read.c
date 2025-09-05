#include <stdbool.h>

#include <disc/read.h>
#include <disc/print_error.h>

#include <sys/asm/out.h>
#include <sys/asm/in.h>

#include <console/print.h>
#include <console/print_hex.h>

#include <defs.h>

static inline bool wait_ready(ata_pio_io_port_t * io_port) {
    volatile union { uint8_t uint8; ata_pio_status_t value; } status = { .uint8 = inb_ptr(&io_port->status), };
    while ((status.value.busy || !status.value.ready) && !status.value.error) status.uint8 = inb_ptr(&io_port->status);
    if (status.value.error) {
        disc_print_error(status.value.error);

        return false;
    }

    return true;
}

uint32_t disc_read28(port_t _io_port, uint8_t drive_select, uint32_t lba, uint8_t sector_count, uint16_t * dest) {
    ata_pio_io_port_t * io_port = (ata_pio_io_port_t *) (intptr_t) _io_port;

    if (lba > 0xFFFFFF) return 1;

    outb_ptr(&io_port->drive_head, drive_select | ((lba >> 24) & 0xF));
    outb_ptr(&io_port->error, 0x00);

    outb_ptr(&io_port->sector_count, sector_count);

    outb_ptr(&io_port->lba_low, lba);
    outb_ptr(&io_port->lba_mid, lba >> 8);
    outb_ptr(&io_port->lba_high, lba >> 16);

    outb_ptr(&io_port->command, ATA_PIO_COMMAND_READ);

    inb_ptr(&io_port->status);
    inb_ptr(&io_port->status);
    inb_ptr(&io_port->status);
    inb_ptr(&io_port->status);

    for (int i = 0; i < sector_count; i++) {
        for (int j = 0; j < 256; j++) {
            while (1) {
                union { uint8_t uint8; ata_pio_status_t value; } status = { .uint8 = inb_ptr(&io_port->status), };

                if (status.value.error) {
                    disc_print_error(status.value.error);

                    return 2;
                }

                if (status.value.data_ready) break;
            }

            *dest = inw_ptr(&io_port->data);

            dest++;
        }
    }

    if (!wait_ready(io_port)) {
        return 3;
    }

    return 0;
}

uint32_t disc_read48(port_t _io_port, uint8_t drive_select, uint64_t lba, uint16_t sector_count, uint16_t * dest) {
    const ata_pio_io_port_t * io_port = (const ata_pio_io_port_t *) (intptr_t) _io_port;

    if (lba > 0xFFFFFFFFFFFF) {
        console_print("DISC_ERROR: 1\n");
        return 1;
    }

    outb_ptr(&io_port->drive_head, drive_select);

    outb_ptr(&io_port->sector_count, sector_count >> 8);
    outb_ptr(&io_port->lba_low, lba >> 24);
    outb_ptr(&io_port->lba_mid, lba >> 32);
    outb_ptr(&io_port->lba_high, lba >> 40);
    outb_ptr(&io_port->sector_count, sector_count & 0xFF);
    outb_ptr(&io_port->lba_low, lba >> 0);
    outb_ptr(&io_port->lba_mid, lba >> 8);
    outb_ptr(&io_port->lba_high, lba >> 16);
    outb_ptr(&io_port->command, ATA_PIO_COMMAND_READ_EXT);

    union { uint8_t uint8; ata_pio_status_t value; } status = { .uint8 = inb_ptr(&io_port->status), };
    for (int i = 0; i < sector_count; i++) {
        while ((status.value.busy || !status.value.ready) && !status.value.error) status.uint8 = inb_ptr(&io_port->status);
        if (status.value.error) {
            uint8_t error = inb_ptr(&io_port->error);
            console_print("DISC_ERROR: ");
            console_print_hex(error);
            console_print("\n");
            return 2;
        }

        for (int j = 0; j < 256; j++) {
            *dest = inw(0x1F0);

            dest++;

            while ((status.value.busy || !status.value.ready) && !status.value.error) status.uint8 = inb_ptr(&io_port->status);
        }

        status.uint8 = inb_ptr(&io_port->status);
    }

    return 0;
}