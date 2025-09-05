#pragma once

#include <console/print.h>

#include <sys/ata/pio.h>

static inline void disc_print_error(uint8_t _error) {
    ata_pio_error_t * error = (ata_pio_error_t *) &_error;

//    console_print("DISC ERROR:\n");
    if (error->aborted_command) console_print("  Aborted Command\n");
    if (error->address_mark_not_found) console_print("  Address not Found\n");
    if (error->bad_block_detected) console_print("  Bad Block\n");
    if (error->id_not_found) console_print("  Id not Found\n");
    if (error->media_change_request) console_print("  Media Change Req\n");
    if (error->media_changed) console_print("  Media Changed\n");
    if (error->track_zero_not_found) console_print("  Track Zero\n");
    if (error->uncorrectable_data_error) console_print("  Data Error\n");
}