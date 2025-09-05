#include <commands/echo.h>

#include <console/print.h>
#include <console/newline.h>
#include <console/put.h>

__CDECL uint32_t command_echo(uint32_t argc, const char ** argv) {
    if (argc <= 1) {
        console_print("ECHO: Invalid arguments.\n");
        return 1;
    }

    uint32_t i;
    for (i = 1; i < argc - 1; i++) {
        console_print(argv[i]);
        console_put(' ');
    }

    console_print(argv[i]);
    console_newline();

    return 0;
}