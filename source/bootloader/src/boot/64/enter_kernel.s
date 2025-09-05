.code32

.global enter_kernel64
enter_kernel64:
    push %ebx

    call boot_long_mode_available
    test %eax, %eax
    jz   .fail_boot

    # enable write protect
    #mov %cr0,       %eax
    #or  $(1 << 16), %eax
    #mov %eax,       %cr0

    # disable write protect
    mov %cr0,       %eax
    and $(~(1 << 16)), %eax
    mov %eax,       %cr0

    # load the paging tables
    mov $pml4t, %eax
    mov %eax,   %cr3

    # enable PAE
    mov %cr4,      %eax
    or  $(1 << 5), %eax
    mov %eax,      %cr4

    # set the long mode bit
    mov   $0xC0000080, %ecx
    rdmsr
    or    $(1 << 8),   %eax
    wrmsr

    # enable paging
    mov %cr0,        %eax
    or  $0x80000000, %eax
    mov %eax,        %cr0

    # load the 64 bit gdt
    lgdt gdt64_ptr

    # load the tss
    mov $0x30, %eax
    ltr %eax

    # load the 64 bit idt
    lidt idt64_ptr

    cli
    jmp $0x10, $entry_64

.code32
.fail_boot:
    mov $0, %eax
    pop %ebx
    ret

.code64
entry_64:
    cli
    mov $0x20, %ax
    mov %ax,   %ds
    mov %ax,   %es
    mov %ax,   %fs
    mov %ax,   %gs
    mov %ax,   %ss

    xor %rax,          %rax
    mov $kernel_entry, %eax
    jmp *%rax

