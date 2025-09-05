#include <disc/ports.h>

#include <sys/ports.h>
#include <sys/pci/pci.h>

port_t disc_io_port;
port_t disc_control_port;

void disc_select_port(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            pci_address_t pci_base = PCI_ADDRESS(bus, device, 0);

            if (pci_exists(pci_base)) {
                uint8_t functions = 1;
                if (pci_is_multifunction(pci_base)) functions = 8;

                for (uint8_t func = 0; func < functions; func++) {
                    pci_address_t pci = PCI_ADDRESS(bus, device, func);

                    if (pci_exists(PCI_ADDRESS(bus, device, func))) {
                        uint16_t class_pair = pci_get_class_pair(pci);

                        if (class_pair == PCI_CLASS_PAIR_IDE_CONTROLLER) {
                            disc_io_port = pci_read_bar(pci, 0) & ~0b11;
                            disc_control_port = pci_read_bar(pci, 1) & ~0b11;

                            if (disc_io_port == 0) disc_io_port = ATA_PIO_PRIMARY;
                            if (disc_control_port == 0) disc_control_port = ATA_PIO_PRIMARY_CONTROL;

                            return;
                        }
                    }
                }
            }
        }
    }
}