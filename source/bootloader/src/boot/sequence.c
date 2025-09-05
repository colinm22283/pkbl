#include <boot/sequence.h>
#include <boot/disc.h>
#include <boot/enter_kernel.h>
#include <boot/kernel.h>
#include <boot/config.h>
#include <boot/graphics_mode.h>
#include <boot/idt64.h>

#include <disc/read.h>
#include <disc/filesystem.h>

#include <console/print.h>
#include <console/print_bool.h>
#include <console/newline.h>
#include <console/put.h>

#include <shell/input.h>

#include <keyboard/getch.h>

#include <heap/init.h>

#include <gdt64.h>
#include <page_tables.h>
#include <memory_map.h>

#include <sys/halt.h>
#include <console/print_hex.h>

#include <pic/mask.h>

static inline void print_init(const char * message) {
    console_print("Init ");
    console_print(message);
    console_print("... ");
}

static inline void print_done() {
    console_print("done!\n");
}

uint32_t boot_sequence_start(const char * kernel_path) {
//    print_init("disc");
//    if (!boot_disc_init()) return 1;
//    print_done();
//    console_print("Disc info:\n  Primary bus found: ");
//    console_print_bool(boot_disc_primary_present);
//    console_print("\n  Secondary bus found: ");
//    console_print_bool(boot_disc_secondary_present);
//    console_print("\n  Bus selected: ");
//    console_print(boot_disc_primary_present ? "PRIMARY" : "SECONDARY");
//    console_print("\n  Drive selected: ");
//    console_print(boot_disc_master_selected ? "MASTER\n" : "SLAVE\n");

    print_init("primary memory region");
    primary_memory_region_t primary_memory_region = get_primary_memory_region();
    if (primary_memory_region.base == 0 && primary_memory_region.length == 0) return 2;
    kernel_region_base = primary_memory_region.base;
    kernel_region_length = primary_memory_region.length;
    print_done();
    console_print("Primary memory region:\n  Start: 0x");
    console_print_hex(primary_memory_region.base);
    console_print("\n  Size:  0x");
    console_print_hex(primary_memory_region.length);
    console_newline();

    print_init("kernel memory");
    if (!boot_disc_load_kernel(kernel_path, primary_memory_region.base)) return 3;
    print_done();
    console_print("  Kernel loaded at physical address 0x");
    console_print_hex(primary_memory_region.base);
    console_newline();

    print_init("paging tables");
    page_tables_init(primary_memory_region.base);
    print_done();
    console_print("  Kernel mapped at virtual address 0x");
    console_print_hex(0xC0000000);
    console_newline();

    shell_ready_to_execute = true;
    // TODO: undo this comment
    console_print("Finalize boot? (y/n)");
    char input = keyboard_getch();
    while (input != 'y' && input != 'n') {
        console_print("\nInvalid selection '");
        console_put(input);
        console_put('\'');
    
        input = keyboard_getch();
    }
    console_newline();
    if (input == 'n') {
        heap_init();
    
        shell_ready_to_execute = false;
        return 4;
    }

    print_init("mask PIC interrupts");
    pic_mask_all();
    print_done();

    print_init("64bit IDT setup");
    idt64_init();
    print_done();

    print_init("BIOS graphics mode");
    boot_switch_graphics_mode();
    print_done();

    bool boot_result;
#ifdef BOOT_CONFIG_64_BIT_ENABLE
    boot_result = enter_kernel64();
#else
    boot_result = enter_kernel32();
#endif

    if (!boot_result) return 5;

    return 0;
}
