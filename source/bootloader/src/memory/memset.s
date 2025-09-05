.code32

.global memset
memset:
    push %ebp
    mov  %esp, %ebp

    movl 8(%esp),  %eax
    movb 12(%esp), %dl
    movl 16(%esp), %ecx

    test %ecx,     %ecx
    jz   .ret

    .loop:
        movb %dl, (%eax)
        inc  %eax

        loop .loop

.ret:
    pop  %ebp
    ret
