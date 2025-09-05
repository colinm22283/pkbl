#include <commands/cat.h>

#include <console/print.h>
#include <console/printn.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

#define CAT_READ_BUFFER_SIZE (1024)

uint32_t cat_file(const char * path) {
    directory_t directory = open_filesystem(KERNEL_LBA_START);
    if (directory == 0) return 2;

    file_t file = open_file_path(directory, path);
    if (file == 0) {
        console_print("Unable to locate file with path \"");
        console_print(path);
        console_print("\"\n");
        return 3;
    }

    file_reader_t file_reader;
    if (!file_reader_init(&file_reader, file)) return 4;

    char buf[CAT_READ_BUFFER_SIZE];
    uint32_t amount_read = 0;
    do {
        amount_read = file_reader_read(&file_reader, buf, CAT_READ_BUFFER_SIZE);
        console_printn(buf, amount_read);
    }
    while (amount_read == CAT_READ_BUFFER_SIZE);

    return 0;
}

__CDECL uint32_t command_cat(uint32_t argc, const char ** argv) {
    if (argc <= 1) {
        console_print("Invalid arguments\nUsage: cat <file 1> <file 2> ... <file n>\n");
        return 1;
    }

    for (uint32_t i = 1; i < argc; i++) {
        uint32_t result = cat_file(argv[i]);
        if (result != 0) return result;
    }

    return 0;
}