.code32

.global memcpy
memcpy:
    push %ebp
    mov  %esp, %ebp

    movl 8(%esp),  %eax
    movl 12(%esp), %edx
    movl 16(%esp), %ecx

    mov  %ebx,     %ebp

    test %ecx,     %ecx
    jz   .ret

    .loop:
        movb (%edx), %bl
        movb %bl,    (%eax)

        inc  %eax
        inc  %edx

        loop .loop

.ret:
    mov  %ebp, %ebx

    pop  %ebp
    ret
