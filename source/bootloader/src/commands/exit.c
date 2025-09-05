#include <commands/exit.h>
#include <console/print.h>
#include <console/clear.h>

#include <sys/halt.h>

__CDECL uint32_t command_exit(__UNUSED uint32_t argc, __UNUSED const char ** argv) {
    console_clear_color(CONSOLE_COLOR(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_BLACK));
    console_print("Ready to power off.\n");

    halt();

//    return 0;
}