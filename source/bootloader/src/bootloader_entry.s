.code32

.section .bootloader_entry, "a"
.global bootloader_entry
bootloader_entry:
    # set the segment registers
    mov $0x10, %ax
    mov %ax,   %ds
    mov %ax,   %es
    mov %ax,   %fs
    mov %ax,   %gs
    mov %ax,   %ss

    # set the stack pointer
    mov $stack_top, %esp

    # check for cpuid support
    pushfl
    popl   %ebx
    mov    %ebx, %ecx
    xor    $(1 << 21), %ebx
    pushl  %ebx
    popfl
    pushfl
    popl   %ebx
    pushl  %ecx
    popfl
    xor    %ecx,    %ebx
    mov    $0x0000, %eax
    jz     boot_error

    # check for extended function support
    mov $0x80000000, %eax
    cpuid
    cmp $0x80000001, %eax
    mov $0x0001, %eax
    jb boot_error

    # check for long mode support
    mov $0x80000001, %eax
    cpuid
    test $(1 << 29), %edx
    mov $0x0002, %ax
    jz boot_error

    # initialize the interrupt table_entries
    call int_init

    # sort the memory map
    call memory_map_sort

    # initialize the heap
    call heap_init

    # init the console
    call console_init

    # init the disc
    call disc_select_port

    # attempt to boot the kernel
    push $default_kernel_path
    call boot_sequence_start

    xor %eax, %eax
    mov %eax, %cr3
    mov %eax, %cr4

    # enter the shell environment if boot fails
    jmp shell_entry


.global test
test:
    mov  %cr0, %eax
    push %eax
    call console_print_hex
    pop  %eax
    call console_newline

    mov  %cr2, %eax
    push %eax
    call console_print_hex
    pop  %eax
    call console_newline

    mov  %cr3, %eax
    push %eax
    call console_print_hex
    pop  %eax
    call console_newline

    mov  %cr4, %eax
    push %eax
    call console_print_hex
    pop  %eax
    call console_newline

    ret
