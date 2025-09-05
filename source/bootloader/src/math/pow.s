.code32

.global math_pow
math_pow:
    push %ebx

    movl 8(%esp),  %edx
    movl 12(%esp), %ecx
    mov  $1,       %eax

    .loop:
        mov  %edx, %ebx
        imul %edx
        mov  %ebx, %edx

        loop .loop

    pop  %ebx
    ret
