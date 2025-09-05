#include <commands/version.h>

#include <console/print.h>

#include <version.h>

__CDECL uint32_t command_version(__MAYBE_UNUSED uint32_t argc, __MAYBE_UNUSED const char ** argv) {
    console_print("PK Bootloader V" VERSION_STRING "\n");
    console_print("Compiled on " __DATE__ "\n");

    return 0;
}