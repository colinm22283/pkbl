OUTPUT_FORMAT("binary");
ENTRY(bootloader16_entry);
SECTIONS {
    . = 0x0500 + 0x404;
    .stack (NOLOAD) : {
        stack_base = .;
        . += 0x07C00 - 0x00500 - 0x404 - 16;
        . = ALIGN(16);
        stack_top = .;
    } > usable_memory_1

    . = 0x7C00;

    bootloader16_entry = .;

    .boot_sector : AT(0x0000) {
        *(.boot_sector);
        *(.gdt32);
        *(.gdt32_ptr);
    } > boot_sector

    .sig 0x7DFE : AT(0x1FE) {
        SHORT(0xAA55);
    } > boot_sector

    . = 0x7E00;

    protected_mode_entry = .;
}