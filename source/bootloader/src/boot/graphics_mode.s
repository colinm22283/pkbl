.code32

.section ".bss_low"
gdt32_buffer: .skip 6

.section ".text"
.global boot_switch_graphics_mode
boot_switch_graphics_mode:
    cli

    sgdt gdt32_buffer

    lgdt gdt16_ptr

    jmp $0x08, $.entry16_protected

.code16
.entry16_protected:
    mov $0x10, %ax
    mov %ax,   %ds
    mov %ax,   %es
    mov %ax,   %fs
    mov %ax,   %gs
    mov %ax,   %ss

    lidt bios_idt_ptr

    mov %cr0,  %eax
    and $0xFE, %al
    mov %eax,  %cr0

    jmp $0x00, $.entry16_real

.entry16_real:
    mov $0x00, %ax
    mov %ax,   %ds
    mov %ax,   %es
    mov %ax,   %fs
    mov %ax,   %gs
    mov %ax,   %ss

    mov $0x0013, %eax
    int $0x10

    lgdt gdt32_buffer

    mov %cr0, %eax
    or  $1, %al
    mov %eax, %cr0

    jmp $0x08, $.entry32

.code32
.entry32:
    mov $0x10, %ax
    mov %ax,   %ds
    mov %ax,   %es
    mov %ax,   %fs
    mov %ax,   %gs
    mov %ax,   %ss

    ret
