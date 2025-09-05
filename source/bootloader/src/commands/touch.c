#include <commands/touch.h>

#include <console/print.h>

#include <disc/filesystem.h>
#include <disc/write.h>
#include <disc/read.h>

#include <boot/kernel.h>

#include <string/strlen.h>

#include <memory/memcpy.h>
#include <memory/memset.h>

#include <sys/ports.h>

__CDECL uint32_t command_touch(uint32_t argc, const char ** argv) {
    if (argc < 2) {
        console_print("Must have at least one argument\n");
        return 1;
    }

    for (uint32_t i = 1; i < argc; i++) {
        uint32_t path_len = strlen(argv[i]);

        char buffer[path_len + 1];
        memcpy(buffer, argv[i], path_len);
        buffer[path_len] = '\0';

        uint32_t j;
        for (j = path_len - 1; j > 0; j--) {
            if (buffer[j] == '/') {
                buffer[j] = '\0';
                j++;
                break;
            }
        }

        directory_t dir = open_filesystem(KERNEL_LBA_START);

        if (j != 0) {
            dir = open_directory_path(dir, buffer);
        }

        if (!create_file(KERNEL_LBA_START, dir, buffer + j)) {
            console_print("Failed to write file \"");
            console_print(argv[i]);
            console_print("\"\n");
            return 2;
        }
    }

    return 0;
}