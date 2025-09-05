#include <commands/boot.h>

#include <console/print.h>

#include <boot/sequence.h>
#include <boot/default_kernel_path.h>

__CDECL uint32_t command_boot(__UNUSED uint32_t argc, __UNUSED const char ** argv) {
    console_print("Beginning boot sequence...\n");

    const char * kernel_path = default_kernel_path;

    if (argc == 2) {
        kernel_path = argv[1];
    }
    else if (argc != 1) {
        console_print("Invalid arguments, usage: boot <?kernel_path>\n");

        return 255;
    }

    return boot_sequence_start(kernel_path);
}