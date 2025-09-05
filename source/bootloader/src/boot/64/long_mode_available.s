.code32

.global boot_long_mode_available
boot_long_mode_available:
    push %ebx

    mov $0x80000000, %eax
    cpuid
    cmp $0x80000001, %eax
    jb  .no_long_mode

    mov  $0x80000001, %eax
    cpuid
    test $(1 << 29),  %edx
    jz   .no_long_mode

    pop %ebx
    mov $1, %eax
    ret

.no_long_mode:
    pop %ebx
    xor %eax, %eax
    ret
