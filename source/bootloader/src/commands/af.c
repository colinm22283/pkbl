#include <commands/af.h>

#include <console/print.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

#include <string/strlen.h>

__CDECL uint32_t command_af(uint32_t argc, const char ** argv) {
    if (argc < 3) {
        console_print("Expected at least 2 arguments.\n");
        return 1;
    }

    file_t file = open_file_path(open_filesystem(KERNEL_LBA_START), argv[argc - 1]);

    if (file == 0) {
        console_print("Unable to open file \"");
        console_print(argv[argc - 1]);
        console_print("\"\n");
        return 2;
    }

    if (!append_file(KERNEL_LBA_START, file, argv[1], strlen(argv[1]))) {
        console_print("Unable to append file \"");
        console_print(argv[argc - 1]);
        console_print("\"\n");
        return 3;
    }

    for (uint32_t i = 2; i < argc - 1; i++) {
        if (!append_file(KERNEL_LBA_START, file, " ", 1)) {
            console_print("Unable to append file \"");
            console_print(argv[argc - 1]);
            console_print("\"\n");
            return 3;
        }

        if (!append_file(KERNEL_LBA_START, file, argv[i], strlen(argv[i]))) {
            console_print("Unable to append file \"");
            console_print(argv[argc - 1]);
            console_print("\"\n");
            return 3;
        }
    }

    if (!append_file(KERNEL_LBA_START, file, "\n", 1)) {
        console_print("Unable to append file \"");
        console_print(argv[argc - 1]);
        console_print("\"\n");
        return 3;
    }

    return 0;
}