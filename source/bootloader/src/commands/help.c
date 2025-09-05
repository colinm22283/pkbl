#include <commands/help.h>

#include <console/console.h>
#include <console/print.h>
#include <console/put.h>
#include <console/clear.h>
#include <console/print_dec.h>
#include <console/newline.h>

#include <string/stoi.h>

const char * const help_text[] = {
    "PKOS BOOTLOADER HELP\n"
    "Compiled on " __DATE__ "\n"
    "\n"
    "This is a temporary help page for the PKOS bootloader V1\n"
    "\n"
    "Command listings ----------- page 1\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n",

    "COMMAND LISTINGS\n"
    "\n"
    "boot ------------------- Boot the operating system\n"
    "calc ------------------- Simple calculator\n"
    "cat -------------------- Dump the data from a file to the console\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
};
const uint32_t help_text_pages = sizeof(help_text) / sizeof(help_text[0]);

__CDECL uint32_t command_help(uint32_t argc, const char ** argv) {
    int32_t page_number = 0;

    if (argc == 2) {
        if (stoi(&page_number, argv[1])) {
            console_put('\"');
            console_print(argv[1]);
            console_print("\", invalid page number\n");
            return 1;
        }

        if (page_number < 0) {
            console_print("Page number cannot be less than zero\n");
            return 2;
        }
    }
    else if (argc > 2) {
        console_print("Invalid arguments\nExpected 0 or 1\n");
        return 3;
    }

    if ((uint32_t) page_number >= help_text_pages) {
        console_print("Page number is out of range\nAvailable pages: ");
        console_print_dec(help_text_pages);
        console_newline();
        return 4;
    }

    console_clear();
    console_print("BEGIN HELP PAGE < ");
    console_print_dec(page_number);
    console_print(" >\n\n");

    console_print(help_text[page_number]);

    console_print("\nEND HELP PAGE < ");
    console_print_dec(page_number);
    console_print(" >\n");

    return 0;
}