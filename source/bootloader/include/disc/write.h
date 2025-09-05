#include <stdbool.h>
#include <stdint.h>

#include <sys/ata/pio.h>

enum {
    DISC_WRITE28_MASTER = 0xE0,
    DISC_WRITE28_SLAVE  = 0xF0,
};

enum {
    DISC_WRITE48_MASTER = 0x40,
    DISC_WRITE48_SLAVE  = 0x50,
};

uint32_t disc_write28(port_t _io_port, uint8_t drive_select, uint64_t lba, uint16_t sector_count, uint16_t * src);
uint32_t disc_write48(port_t _io_port, uint8_t drive_select, uint64_t lba, uint16_t sector_count, uint16_t * src);