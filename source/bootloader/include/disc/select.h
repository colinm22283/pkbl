#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <sys/ata/pio.h>

#define DISC_SELECT_MASTER (0xA0)
#define DISC_SELECT_SLAVE  (0xB0)

bool disc_select(port_t _io_port, uint8_t drive_id);