#include <commands/less.h>

#include <console/print.h>
#include <console/put.h>
#include <console/console.h>

#include <keyboard/getch.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

__CDECL uint32_t command_less(uint32_t argc, const char ** argv) {
    if (argc != 2) {
        console_print("Invalid arguments\nUsage: less <file>");
        return 1;
    }

    directory_t directory = open_filesystem(KERNEL_LBA_START);
    if (directory == 0) return 2;

    file_t file = open_file_path(directory, argv[1]);
    if (file == 0) {
        console_print("Unable to locate file with path \"");
        console_print(argv[1]);
        console_print("\"\n");
        return 3;
    }

    file_reader_t file_reader;
    if (!file_reader_init(&file_reader, file)) return 4;

    uint32_t newline_count = 0;

    char buf;
    uint32_t amount_read = 0;
    do {
        amount_read = file_reader_read(&file_reader, &buf, 1);
        console_put(buf);

        if (buf == '\n') {
            newline_count++;

            if (newline_count == CONSOLE_HEIGHT) {
                newline_count = 0;

                keyboard_getch();
            }
        }
    }
    while (amount_read == 1);

    return 0;
}