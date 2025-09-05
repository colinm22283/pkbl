#include <commands/rm.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

#include <console/print.h>
#include <console/print_hex.h>
#include <console/newline.h>

__CDECL uint32_t command_rm(uint32_t argc, const char ** argv) {
    if (argc < 2) {
        console_print("Invalid arguments. Expected at least 1\n");

        return 1;
    }

    for (uint32_t i = 1; i < argc; i++) {
        filesystem_page_address_t node = open_node_path(
            open_filesystem(KERNEL_LBA_START),
            argv[1]
        );

        console_print("0x");
        console_print_hex(node);
        console_newline();

        if (!delete_node(
            KERNEL_LBA_START,
            node
        )) {
            console_print("Failed to delete \"");
            console_print(argv[i]);
            console_print("\"\n");

            return 2;
        }
    }

    return 0;
}