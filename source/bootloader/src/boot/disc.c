#include <boot/disc.h>
#include <boot/kernel.h>

#include <disc/detect.h>
#include <disc/reset.h>
#include <disc/select.h>
#include <disc/read.h>
#include <disc/filesystem.h>

#include <sys/ports.h>

#include <console/print.h>
#include <console/print_hex.h>
#include <console/newline.h>
#include <console/print_bool.h>

#include <sys/asm/hlt.h>
#include <sys/asm/out.h>

#include <memory/memset.h>
#include <memory/memcpy.h>

bool boot_disc_primary_present;
bool boot_disc_secondary_present;
bool boot_disc_master_selected;

port_t io_port;
port_t control_port;

bool boot_disc_init() {
    boot_disc_primary_present = disc_detect(ATA_PIO_PRIMARY);
    boot_disc_secondary_present = disc_detect(ATA_PIO_SECONDARY);

    if (boot_disc_primary_present) disc_reset(ATA_PIO_PRIMARY_CONTROL);
    if (boot_disc_secondary_present) disc_reset(ATA_PIO_SECONDARY_CONTROL);

    if (boot_disc_primary_present) {
        io_port = ATA_PIO_PRIMARY;
        control_port = ATA_PIO_PRIMARY_CONTROL;
    }
    else if (boot_disc_secondary_present) {
        io_port = ATA_PIO_SECONDARY;
        control_port = ATA_PIO_SECONDARY_CONTROL;
    }
    else return false;

    if (disc_select(io_port, DISC_SELECT_MASTER)) boot_disc_master_selected = true;
    else if (disc_select(io_port, DISC_SELECT_SLAVE)) boot_disc_master_selected = false;
    else return false;

    ata_pio_control_port_t * control_port_ptr = (ata_pio_control_port_t *) (intptr_t) control_port;

    outb_ptr(&control_port_ptr->device_control, 0b10);

    return true;
}

bool boot_disc_load_kernel(const char * kernel_path, uint32_t kernel_load_point) {
    disc_reset(ATA_PIO_PRIMARY_CONTROL);

    outb_ptr(&((ata_pio_control_port_t *) ATA_PIO_PRIMARY_CONTROL)->device_control, 0b10);

    directory_t root_directory = open_filesystem(KERNEL_LBA_START);
    if (root_directory == 0) {
        console_print("Failed to open FS\n");

        return false;
    }

    file_t kernel_file = open_file_path(root_directory, kernel_path);
    if (kernel_file == 0) {
        console_print("Failed to open path\n");

        return false;
    }

    file_reader_t reader;
    if (!file_reader_init(&reader, kernel_file)) {
        console_print("Failed to init reader\n");

        return false;
    }

    char * kernel_ptr = (char *) kernel_load_point;

    uint64_t read_bytes;
    while ((read_bytes = file_reader_read(&reader, kernel_ptr, FILESYSTEM_FILE_DATA_PAGE_SIZE)) > 0) {
        kernel_ptr += read_bytes;
    }

    return true;
}