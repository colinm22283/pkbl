#pragma once

#include <stdint.h>

#include <sys/ata/pio.h>

enum {
    DISC_READ28_MASTER = 0xE0,
    DISC_READ28_SLAVE  = 0xF0,
};

enum {
    DISC_READ48_MASTER = 0x40,
    DISC_READ48_SLAVE  = 0x50,
};

uint32_t disc_read28(port_t _io_port, uint8_t drive_select, uint32_t lba, uint8_t sector_count, uint16_t * dest);
uint32_t disc_read48(port_t _io_port, uint8_t drive_select, uint64_t lba, uint16_t sector_count, uint16_t * dest);