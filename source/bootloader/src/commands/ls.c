#include <commands/ls.h>

#include <console/print.h>
#include <console/newline.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

__CDECL uint32_t command_ls(uint32_t argc, const char ** argv) {
    if (argc > 2) {
        console_print("Invalid arguments\nUsage: ls <directory>\n");
        return 1;
    }

    directory_t root = open_filesystem(KERNEL_LBA_START);
    if (root == 0) {
        console_print("Unable to open filesystem\n");
        return 2;
    }
    directory_t directory;
    if (argc == 1) {
        directory = root;
    }
    else {
        directory = open_directory_path(root, argv[1]);

        if (directory == 0) {
            console_print("Unable to locate directory \"");
            console_print(argv[1]);
            console_print("\"\n");
            return 3;
        }
    }

    char name_buffer[FILESYSTEM_NAME_MAX_SIZE];

    directory_iterator_t iterator;
    directory_iterator_init(&iterator, directory);
    while (true) {
        filesystem_page_address_t new_page;
        filesystem_directory_entry_type_t entry_type = directory_iterator_next(&iterator, &new_page);

        if (entry_type == FS_DET_NONE) break;
        else if (entry_type == FS_DET_DIRECTORY) {
            console_print("<DIR>  ");
            get_directory_name(new_page, name_buffer);
            console_print(name_buffer);
            console_newline();
        }
        else if (entry_type == FS_DET_FILE) {
            console_print("<FILE> ");
            get_file_name(new_page, name_buffer);
            console_print(name_buffer);
            console_newline();
        }
    }

    return 0;
}