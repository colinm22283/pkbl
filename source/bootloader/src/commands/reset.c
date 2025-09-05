#include <commands/reset.h>

#include <console/clear.h>

__CDECL uint32_t command_reset(__MAYBE_UNUSED uint32_t argc, __MAYBE_UNUSED const char ** argv) {
    console_clear_color(CONSOLE_COLOR(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_BLACK));

    return 0;
}