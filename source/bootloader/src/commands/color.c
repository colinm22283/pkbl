#include <commands/color.h>

#include <console/print.h>
#include <console/color_lut.h>
#include <console/set_color.h>

__CDECL uint32_t command_color(uint32_t argc, const char ** argv) {
    if (argc != 3) {
        console_print("Invalid arguments\nUsage: COLOR <fg> <bg>\n");
        return 1;
    }

    uint8_t fg_color = CONSOLE_COLOR_WHITE, bg_color = CONSOLE_COLOR_BLACK;

    fg_color = console_color_str_lookup(argv[1]);
    if (fg_color == 0xFF) {
        console_print("Invalid FG color!\n");
        return 2;
    }

    bg_color = console_color_str_lookup(argv[2]);
    if (bg_color == 0xFF) {
        console_print("Invalid BG color!\n");
        return 3;
    }

    console_set_color(CONSOLE_COLOR(fg_color, bg_color));

    return 0;
}