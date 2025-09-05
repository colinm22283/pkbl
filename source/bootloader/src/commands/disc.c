#include <commands/disc.h>

#include <console/print.h>
#include <console/print_dec.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

__CDECL uint32_t command_disc(__MAYBE_UNUSED uint32_t argc, __MAYBE_UNUSED const char ** argv) {
    uint64_t size_on_disc;

    if (!stat_filesystem(KERNEL_LBA_START, &size_on_disc)) {
        console_print("Unable to stat filesystem\n");

        return 1;
    }

    console_print("Filesystem size on disk: ");
    console_print_dec(size_on_disc);
    console_print(" bytes\n");

    return 0;
}
