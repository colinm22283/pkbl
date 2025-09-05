#pragma once

#include <stdbool.h>

#include <sys/ata/pio.h>
#include <sys/asm/in.h>

static inline bool wait_ready(port_t _io_port) {
    const ata_pio_io_port_t * io_port = (const ata_pio_io_port_t *) (intptr_t) _io_port;

    union { uint8_t uint8; ata_pio_status_t value; } status = { .uint8 = inb_ptr(&io_port->status), };
    while ((status.value.busy || !status.value.ready) && !status.value.error) status.uint8 = inb_ptr(&io_port->status);
    if (status.value.error) return false;

    return true;
}