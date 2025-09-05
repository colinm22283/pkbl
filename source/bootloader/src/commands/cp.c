#include <commands/cp.h>

#include <console/print.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

#include <string/strlen.h>

__CDECL uint32_t command_cp(uint32_t argc, const char ** argv) {
    if (argc != 3) {
        console_print("Invalid arguments\nUsage: cp <src> <dst>\n");
        return 1;
    }

    directory_t root = open_filesystem(KERNEL_LBA_START);

    file_t src_file = open_file_path(root, argv[1]);
    if (src_file == 0) {
        console_print("Invalid source file\r");
        return 2;
    }

    file_t dst_file = open_file_path(root, argv[2]);
    if (dst_file == 0) {
        console_print("Invalid destination file\r");
        return 3;
    }

    file_reader_t file_reader;
    if (!file_reader_init(&file_reader, src_file)) {
        console_print("Unable to read from source file\n");
        return 4;
    }

    uint32_t amount_read;
    char buffer[1024];
    while ((amount_read = file_reader_read(&file_reader, buffer, 1024)) != 0) {
        append_file(root, dst_file, buffer, amount_read);
    }

    return 0;
}