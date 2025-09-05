.code16

.section .boot_sector, "a"
.global bootloader16_entry
bootloader16_entry:
    # disable interrupts
    cli

    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss

    # set a stack pointer
    mov $stack_top, %sp

    # set console graphics mode
	mov $0x0003, %ax
	int $0x10

    # disable the cursor
    mov $0x01,   %ah
    mov $0x2000, %cx
    int $0x10

    # check for extended bios functions
    mov $0x41,   %ah
    mov $0x55AA, %bx
    mov $0x80,   %dl
    int $0x13
    jc  bios_extensions_not_found

    # load the bootloader into memory
    mov $0x42,               %ah
    mov $0x80,               %dl
    mov $extended_read_data, %si
    int $0x13
    jc  bootloader_load_error

    # load the memory map
    mov $0xE820,     %eax
    xor %ebx,        %ebx
    mov $24,         %ecx
    mov $0x534D4150, %edx
    mov $memory_map, %di

    pushw $0
    mov   %sp, %bp

    mov   $0,  %si

    .memory_map_loop:
        push %di
        int  $0x15
        pop  %di
        jc   ..memory_map_loop_exit
        test %ebx,        %ebx
        jz   ..memory_map_loop_exit

        movw (%bp), %ax
        inc  %ax
        movw %ax,    (%bp)

        mov  $0xE820,     %eax
        mov  $24,         %ecx
        mov  $0x534D4150, %edx
        add  $24,         %di

        inc  %si
        cmp  $42,        %si
        jbe  .memory_map_loop
    # fault if the maximum amount of sections are reached
    jmp detect_memory_failure
    ..memory_map_loop_exit:

    movw (%bp), %ax
    dec  %ax
    movw %ax,    (memory_map_size)
    xor  %ax,    %ax
    movw %ax,    (memory_map_size + 2)

    add  $2,     %esp

    # enable A20 line
    inb $0x92, %al
    or  $0x02, %al
    out %al,   $0x92
    #stc
    #mov $0x2401, %ax
    #int $0x15
    #jc  a20_failure

    # load the gdt
    lgdt gdt32_ptr

    # enable the gdt
    mov %cr0, %eax
    or  $1, %al
    mov %eax, %cr0

    # enter the bootloader in protected mode
    ljmp $0x08, $protected_mode_entry

bios_extensions_not_found:
    mov $25, %cx
    mov $bios_extensions_not_found_str, %bp
    jmp print_error_string
bootloader_load_error:
    mov $23, %cx
    mov $bootloader_load_error_str, %bp
    jmp print_error_string
a20_failure:
    mov $22, %cx
    mov $a20_failure_str, %bp
    jmp print_error_string
detect_memory_failure:
    mov $32, %cx
    mov $detect_memory_failure_str, %bp

# bp is the string
# cx is the string length
print_error_string:
    # print the message
    mov $0x1301,                        %ax
    mov  $0x001F,                       %bx
    xor  %dx,  %dx
    int  $0x10

    # clear the screen
    mov %cx, %dx
    mov $(80 * 25), %cx
    sub %dx, %cx
    mov $0x0900, %ax
    mov $0x001F, %bx
    int $0x10

    hlt

bios_extensions_not_found_str: .string "BIOS EXTENSIONS NOT FOUND"
bootloader_load_error_str:     .string "BOOTLOADER LOAD ERROR"
a20_failure_str:               .string "A20 LINE ENABLE FAILED"
detect_memory_failure_str:      .string "MEMORY DETECT EXCEDED MAX AMOUNT"

extended_read_data:
    .byte  0x10
    .byte  0x00
    .short 63 # blocks to transfer
    .short 0x7E00
    .short 0x0000
    .int  1
    .int  0
