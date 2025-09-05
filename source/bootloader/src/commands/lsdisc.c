#include <commands/lsdisc.h>

#include <disc/detect.h>
#include <disc/select.h>
#include <disc/reset.h>
#include <disc/filesystem.h>
#include <disc/read.h>
#include <disc/print_error.h>
#include <keyboard/getch.h>

#include <boot/kernel.h>

#include <console/print.h>
#include <console/print_hex.h>

#include <sys/ata/pio.h>
#include <sys/ports.h>
#include <sys/asm/out.h>
#include <sys/asm/in.h>
#include <sys/pci/pci.h>

#define PRIMARY_IO_PORT ((ata_pio_io_port_t *) ATA_PIO_PRIMARY)
#define SECONDARY_IO_PORT ((ata_pio_io_port_t *) ATA_PIO_SECONDARY)
#define PRIMARY_CONTROL_PORT ((ata_pio_control_port_t *) ATA_PIO_PRIMARY_CONTROL)
#define SECONDARY_CONTROL_PORT ((ata_pio_control_port_t *) ATA_PIO_SECONDARY_CONTROL)

enum {
    DEVICE_DETECT_DRIVE_ID_MASTER = 0xA0,
    DEVICE_DETECT_DRIVE_ID_SLAVE = 0xB0,
};

typedef enum {
    CHECK_DEVICE_NOT_PRESENT,
    CHECK_DEVICE_ATA_PIO,
    CHECK_DEVICE_ATAPI,
} check_device_t;

static inline check_device_t check_drive(ata_pio_io_port_t * io_port, uint8_t drive_id) {
    outb_ptr(&io_port->drive_head, drive_id);
    outb_ptr(&io_port->lba_low, 0);
    outb_ptr(&io_port->lba_mid, 0);
    outb_ptr(&io_port->lba_high, 0);
    outb_ptr(&io_port->command, ATA_PIO_COMMAND_IDENTIFY);

    union { ata_pio_status_t bits; uint8_t value; } first_status = { .value = inb_ptr(&io_port->status), };

    if (first_status.bits.error > 0) {
        disc_print_error(first_status.bits.error);
        return CHECK_DEVICE_ATAPI;
    }

    if (first_status.value == 0) return CHECK_DEVICE_NOT_PRESENT;

    union { ata_pio_status_t bits; uint8_t value; } status = { .value = inb_ptr(&io_port->status), };
    while (status.bits.busy) status.value = inb_ptr(&io_port->status);

    status.value = inb_ptr(&io_port->status);
    while (!status.bits.data_ready && !status.bits.error) status.value = inb_ptr(&io_port->status);

    if (status.bits.error) {
        return CHECK_DEVICE_NOT_PRESENT;
    }

    return CHECK_DEVICE_ATA_PIO;
}

__CDECL uint32_t command_lsdisc(uint32_t argc, const char ** argv) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            if (pci_exists(PCI_ADDRESS(bus, device, 0))) {
                uint8_t functions = 1;
                if (pci_is_multifunction(PCI_ADDRESS(bus, device, 0))) functions = 8;

                for (uint8_t func = 0; func < functions; func++) {
                    if (pci_exists(PCI_ADDRESS(bus, device, func))) {
                        uint16_t class_pair = pci_get_class_pair(PCI_ADDRESS(bus, device, func));

                        if (class_pair == 0x0101) {
                            console_print("IDE Controller: 0x");
                            console_print_hex(bus);
                            console_print(", 0x");
                            console_print_hex(device);
                            console_print(", 0x");
                            console_print_hex(class_pair);
                            console_print("\n");

                            console_print("Header Type: 0x");
                            console_print_hex(pci_read_byte(PCI_ADDRESS(bus, device, func), 0xE));
                            console_print("\n");

                            console_print("ProgIF: 0x");
                            console_print_hex(pci_read_byte(PCI_ADDRESS(bus, device, func), 0x9));
                            console_print("\n");

                            uint16_t primary_io = pci_read_bar(PCI_ADDRESS(bus, device, func), 0) & ~0b11;
                            uint16_t secondary_io = pci_read_bar(PCI_ADDRESS(bus, device, func), 2) & ~0b11;

                            console_print("Primary IO Port: 0x");
                            console_print_hex(primary_io);
                            console_print("\n");

                            console_print("Primary Control Port: 0x");
                            console_print_hex(pci_read_bar(PCI_ADDRESS(bus, device, func), 1));
                            console_print("\n");

                            console_print("Secondary IO Port: 0x");
                            console_print_hex(secondary_io);
                            console_print("\n");

                            console_print("Secondary Control Port: 0x");
                            console_print_hex(pci_read_bar(PCI_ADDRESS(bus, device, func), 3));
                            console_print("\n");

                            console_print("Primary Status: ");
                            console_print_hex(inb(primary_io + 7));
                            console_print("\n");

                            console_print("Secondary Status: ");
                            console_print_hex(inb(secondary_io + 7));
                            console_print("\n");

                            console_print("Primary Error: ");
                            console_print_hex(inb(primary_io + 1));
                            console_print("\n");

                            console_print("Secondary Error: ");
                            console_print_hex(inb(secondary_io + 1));
                            console_print("\n");
                        }
                    }
                }
            }
        }
    }

//    console_print("Primary\n");
//
//    {
//        check_device_t result = check_drive(PRIMARY_IO_PORT, DEVICE_DETECT_DRIVE_ID_MASTER);
//
//        console_print("  Master: ");
//        if (result == CHECK_DEVICE_NOT_PRESENT) console_print("N/A\n");
//        else if (result == CHECK_DEVICE_ATA_PIO) console_print("ATA PIO\n");
//        else if (result == CHECK_DEVICE_ATAPI) console_print("ATAPI\n");
//    }
//
//    {
//        check_device_t result = check_drive(PRIMARY_IO_PORT, DEVICE_DETECT_DRIVE_ID_SLAVE);
//
//        console_print("  Slave: ");
//        if (result == CHECK_DEVICE_NOT_PRESENT) console_print("N/A\n");
//        else if (result == CHECK_DEVICE_ATA_PIO) console_print("ATA PIO\n");
//        else if (result == CHECK_DEVICE_ATAPI) console_print("ATAPI\n");
//    }
//
//    console_print("Secondary\n");
//
//    {
//        check_device_t result = check_drive(SECONDARY_IO_PORT, DEVICE_DETECT_DRIVE_ID_MASTER);
//
//        console_print("  Master: ");
//        if (result == CHECK_DEVICE_NOT_PRESENT) console_print("N/A\n");
//        else if (result == CHECK_DEVICE_ATA_PIO) console_print("ATA PIO\n");
//        else if (result == CHECK_DEVICE_ATAPI) console_print("ATAPI\n");
//    }
//
//    {
//        check_device_t result = check_drive(SECONDARY_IO_PORT, DEVICE_DETECT_DRIVE_ID_SLAVE);
//
//        console_print("  Slave: ");
//        if (result == CHECK_DEVICE_NOT_PRESENT) console_print("N/A\n");
//        else if (result == CHECK_DEVICE_ATA_PIO) console_print("ATA PIO\n");
//        else if (result == CHECK_DEVICE_ATAPI) console_print("ATAPI\n");
//    }

    static char temp[513];

    {
        uint32_t result = disc_read28(ATA_PIO_PRIMARY, DISC_READ28_MASTER, KERNEL_LBA_START, 1, (void *) &temp);

        console_print("Result: 0x");
        console_print_hex(result);
        console_print("\n");

        temp[512] = '\0';
        console_print("TEST: ");
        console_print(temp);
        console_print("\n");
    }

    return 0;
}