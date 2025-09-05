.code32

.global strcmpn
strcmpn:
    push %ebx

    movl 8(%esp),  %ebx # a
    movl 12(%esp),  %edx # b
    movl 16(%esp), %ecx # n

    xor  %eax,     %eax

    .loop:
        movb (%ebx), %al
        movb (%edx), %ah

        inc %ebx
        inc %edx

        cmp %al, %ah
        jne .exit

        test %al, %ah
        jz   .exit

        loop .loop

.exit:
    sub %ah, %al
    xor %ah, %ah

    pop %ebx
    ret