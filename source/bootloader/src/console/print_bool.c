#include <console/print_bool.h>
#include <console/print.h>

void console_print_bool(bool value) {
    if (value) console_print("true");
    else console_print("false");
}