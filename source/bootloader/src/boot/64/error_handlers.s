.code64

.section .data

error_loc: .quad 0xB8000
newline_loc: .quad 0xB8000 + 160

.section .rodata

unhandled_message: .asciz "Unhandled Error\n"
gpf_message: .asciz "General Protection Fault\n"
pf_message: .asciz "Page Fault\n"
present_message: .asciz "Not Present\n"
write_message: .asciz "Write\n"
user_message: .asciz "User\n"
res_write_message: .asciz "Reserved Write\n"
if_message: .asciz "Instruction Fetch\n"

newline: .asciz "\n"

hex_chars: .string "0123456789ABCDEF"

.section .text

error_handler_print: # rdi: str
    push %rbx

    mov $error_loc,   %ecx
    mov $newline_loc, %edx

    movq (%ecx), %rax

    .ehp_loop:
        cmpb $0, (%rdi)
        je .ehp_end

        cmpb $10, (%rdi)
        jne ..ehp_nonewline
            movq (%edx), %rax
            add  $160,   %rax
            movq %rax,   (%edx)
            sub  $160,   %rax

            jmp ..ehp_continue
        ..ehp_nonewline:

        movb (%rdi), %bl
        movb %bl,    (%rax)

        add $2, %rax

        ..ehp_continue:

        inc %rdi

        jmp .ehp_loop

    .ehp_end:

    movq %rax, (%ecx)

    pop %rbx

    ret

error_handler_print_hex:
    sub $1, %rsp

    movb $0, (%rsp)

    sub $1, %rsp

    mov %rsp, %r8

    sub $15, %rsp

    mov $16, %rcx

    mov %rdi, %rax

    .ehph_loop:
        xor %rdx, %rdx
        mov $16, %r9
        div %r9

        add $hex_chars, %edx

        movb (%edx), %dl

        movb %dl, (%r8)
        sub  $1,  %r8

        loop .ehph_loop

    mov %rsp, %rdi
    call error_handler_print

    add $17, %rsp

    ret

.global gpf_handler64
gpf_handler64:
    mov $gpf_message, %edi
    call error_handler_print

    cli
    hlt

.global pf_handler64
pf_handler64:
    mov  $pf_message, %edi
    call error_handler_print

    movq (%rsp), %rax
    and  $0b000001, %rax
    cmp  $0b000001, %rax
    je   .no_present
        mov  $present_message, %edi
        call error_handler_print
    .no_present:

    movq (%rsp), %rax
    and  $0b000010, %rax
    cmp  $0, %rax
    je   .no_write
        mov  $write_message, %edi
        call error_handler_print
    .no_write:

    movq (%rsp), %rax
    and  $0b000100, %rax
    cmp  $0, %rax
    je   .no_user
        mov  $user_message, %edi
        call error_handler_print
    .no_user:

    movq (%rsp), %rax
    and  $0b001000, %rax
    cmp  $0, %rax
    je   .no_res_write
        mov  $res_write_message, %edi
        call error_handler_print
    .no_res_write:

    movq (%rsp), %rax
    and  $0b010000, %rax
    cmp  $0, %rax
    je   .no_if
        mov  $if_message, %edi
        call error_handler_print
    .no_if:

    pop %rax

    mov  %cr2, %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    pop  %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    pop  %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    pop  %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    pop  %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    pop  %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    mov  %rsp, %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    mov  %cs, %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    mov  %ds, %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    mov  %ss, %edi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    mov  %cr3, %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    pushf
    pop  %rdi
    call error_handler_print_hex
    mov  $newline, %edi
    call error_handler_print

    cli
    hlt

.global unhandled_handler64
unhandled_handler64:
    mov $unhandled_message, %edi
    call error_handler_print

    cli
    hlt
