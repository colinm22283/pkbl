.code32

.section .bootloader, "a"

CONSOLE_WIDTH  = 80
CONSOLE_HEIGHT = 25

console_ptr: .long 0xB8000
newline_ptr: .long 0xB8000 + 160

# overwrites the 'a' register
print_string:
    push %ebx
    push %ecx

    movl (console_ptr), %ebx

    movb (%eax), %cl

    .loop:
        movb %cl, (%ebx)

        inc  %eax
        add  $2,  %ebx

        movb (%eax), %cl

        test %cl,    %cl
        jnz  .loop

    movl %ebx, (console_ptr)

    pop %ecx
    pop %ebx
    ret

print_hex:
    push %ebx
    push %ecx

    xor  %ebx, %ebx

    movl (console_ptr), %ecx
    add  $6,            %ecx

    mov  %al,       %bl
    and  $0xF,      %bl
    add  $.hex_lut, %ebx
    movb (%ebx),    %bl
    movb %bl,       (%ecx)
    sub  $2,        %ecx
    shr  $4,        %ax

    mov  %al,       %bl
    and  $0xF,      %bl
    add  $.hex_lut, %ebx
    movb (%ebx),    %bl
    movb %bl,       (%ecx)
    sub  $2,        %ecx
    shr  $4,        %ax

    mov  %al,       %bl
    and  $0xF,      %bl
    add  $.hex_lut, %ebx
    movb (%ebx),    %bl
    movb %bl,       (%ecx)
    sub  $2,        %ecx
    shr  $4,        %ax

    mov  %al,       %bl
    and  $0xF,      %bl
    add  $.hex_lut, %ebx
    movb (%ebx),    %bl
    movb %bl,       (%ecx)

    add  $8,        %ecx
    movl %ecx,      (console_ptr)

    pop  %ecx
    pop  %ebx
    ret
    .hex_lut: .string "0123456789ABCDEF"

print_newline:
    push %eax

    movl (newline_ptr), %eax
    movl %eax,          (console_ptr)
    add  $160,          %eax
    movl %eax,          (newline_ptr)

    pop  %eax
    ret

.global boot_error
boot_error:
    # clear the screen
    mov $0xB8000,                          %ebx
    mov $(CONSOLE_HEIGHT * CONSOLE_WIDTH), %ecx
    .clear_loop:
        movw $0x1F00,       (%ebx)
        add  $2,          %ebx
        loop .clear_loop

    mov %eax, %edi

    mov  $error_string_1, %eax
    call print_string

    mov  %edi, %eax
    call print_hex

    call print_newline
    call print_newline

    mov $error_table, %ebx
    mov (%ebx, %edi, 4), %eax
    call print_string

    hlt

error_string_1: .asciz "BOOT ERROR 0x"

error_table:
    .long cpuid_not_supported
    .long extended_cpuid_not_supported
    .long long_mode_not_supported

cpuid_not_supported: .asciz "CPUID FUNCTIONS ARE NOT SUPPORTED"
extended_cpuid_not_supported: .asciz "EXTENDED CPUID FUNCTIONS ARE NOT SUPPORTED"
long_mode_not_supported: .asciz "LONG MODE IS NOT SUPPORTED BY THE CPU"
