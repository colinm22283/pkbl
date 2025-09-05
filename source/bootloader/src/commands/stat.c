#include <commands/stat.h>

#include <console/print.h>
#include <console/print_dec.h>
#include <console/newline.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

__CDECL uint32_t command_stat(uint32_t argc, const char ** argv) {
    if (argc != 2) {
        console_print("Invalid arguments!\nUsage: stat <path>\n");

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

    file_stat_result_t stat_result;
    if (!stat_file(&stat_result, file)) {
        console_print("Error while performing stat on file.\n");
        return 4;
    }

    if (stat_result.type == FILESYSTEM_PAGE_TYPE_FILE) {
        console_print("File stats:\n");
        console_print("  Size: ");
        console_print_dec(stat_result.size);
        console_print(" bytes");
    }
    else console_print("Directory stats:");

    console_print("\n  Size on disc: ");
    console_print_dec(stat_result.size_on_disc);
    console_print(" bytes\n");

    return 0;
}