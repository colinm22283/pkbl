#include <disc/detect.h>

#include <sys/asm/in.h>
#include <sys/ata/pio.h>

bool disc_detect(port_t _io_port) {
    const ata_pio_io_port_t * io_port = (const ata_pio_io_port_t *) (intptr_t) _io_port;

    return inb_ptr(&io_port->status) != 0xFF;
}