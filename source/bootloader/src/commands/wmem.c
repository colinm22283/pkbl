#include <commands/wmem.h>

#include <console/print.h>

#include <string/stoi.h>

__CDECL uint32_t command_wmem(uint32_t argc, const char ** argv) {
    if (argc != 3) {
        console_print("Invalid arguments!\nUsage wmem <character> <address>\n");
        return 1;
    }

    int32_t character;

    if (stoi(&character, argv[1])) {
        console_print("Invalid character!\n");
        return 2;
    }
    if (character > 255 || character < 0) {
        console_print("Invalid character!\n");
        return 3;
    }

    int32_t address;
    if (stoi(&address, argv[2])) {
        console_print("Invalid address!\n");
        return 4;
    }

    char * pointer = (char *) address;
    *pointer = (char) character;

    return 0;
}