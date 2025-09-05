#include <disc/write.h>
#include <disc/wait_ready.h>

#include <sys/asm/out.h>

uint32_t disc_write28(port_t _io_port, uint8_t drive_select, uint64_t lba, uint16_t sector_count, uint16_t * src) {
    const ata_pio_io_port_t * io_port = (const ata_pio_io_port_t *) (intptr_t) _io_port;

    if (lba > 0xFFFFFF) return false;

    outb_ptr(&io_port->drive_head, drive_select | ((lba >> 24) & 0xF));

    outb_ptr(&io_port->sector_count, sector_count);

    outb_ptr(&io_port->lba_low, lba);
    outb_ptr(&io_port->lba_mid, lba >> 8);
    outb_ptr(&io_port->lba_high, lba >> 16);

    outb_ptr(&io_port->command, ATA_PIO_COMMAND_WRITE);

    for (int i = 0; i < sector_count; i++) {
        if (!wait_ready(_io_port)) {
            return 1;
        }

        for (int j = 0; j < 256; j++) {
            outw_ptr(&io_port->data, *src);

            src++;

            if (!wait_ready(_io_port)) {
                return 2;
            }
        }

        outb_ptr(&io_port->command, ATA_PIO_COMMAND_CACHE_FLUSH);
    }

    if (!wait_ready(_io_port)) {
        return 3;
    }

    return 0;
}

uint32_t disc_write48(port_t _io_port, uint8_t drive_select, uint64_t lba, uint16_t sector_count, uint16_t * src) {
    const ata_pio_io_port_t * io_port = (const ata_pio_io_port_t *) (intptr_t) _io_port;

    if (lba > 0xFFFFFFFFFFFF) return false;

    outb_ptr(&io_port->drive_head, drive_select);

    outb_ptr(&io_port->sector_count, sector_count >> 8);
    outb_ptr(&io_port->lba_low, lba >> 24);
    outb_ptr(&io_port->lba_mid, lba >> 32);
    outb_ptr(&io_port->lba_high, lba >> 40);
    outb_ptr(&io_port->sector_count, sector_count & 0xFF);
    outb_ptr(&io_port->lba_low, lba >> 0);
    outb_ptr(&io_port->lba_mid, lba >> 8);
    outb_ptr(&io_port->lba_high, lba >> 16);
    outb_ptr(&io_port->command, ATA_PIO_COMMAND_WRITE_EXT);

    for (int i = 0; i < sector_count; i++) {
        if (!wait_ready(_io_port)) {
            return 1;
        }

        for (int j = 0; j < 256; j++) {
            outw_ptr(&io_port->data, *src);

            src++;

            if (!wait_ready(_io_port)) {
                return 2;
            }
        }
    }

    if (!wait_ready(_io_port)) {
        return 3;
    }

    return 0;
}