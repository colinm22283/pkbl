.code32

.global stoi
stoi:
    push %ebp
    push %ebx
    push %edi
    mov  %esp, %ebp

    movl 20(%esp), %edx

    movb (%edx), %cl
    test %cl,    %cl
    jnz  .not_empty
        movl 16(%esp), %eax
        movl $0,      (%eax)
        xor  %eax,    %eax
        pop  %edi
        pop  %ebx
        pop  %ebp
        ret
    .not_empty:

    mov  %esp, %ebx
    sub  $12,  %esp

    dec  %ebx
    movb $0x00, (%ebx)
    .find_end_loop:
        dec  %ebx

        movb (%edx), %cl
        movb %cl,    (%ebx)

        inc  %edx

        test %cl,    %cl
        jnz  .find_end_loop

    mov %ebx, %edx # edx points to the beginning of the reversed string - 1

    # eax will be scratch
    xor  %ebx, %ebx # ebx will be the accumulator
    mov  $1,   %ecx # ecx will be the multiplier
    .loop:
        inc  %edx

        xor  %eax,   %eax
        movb (%edx), %al
        test %al, %al
        jz   .loop_exit
        cmp  $'0',   %al
        jge  ..greater_than_zero
        ..invalid_char:
            cmp  $'-', %al
            jne  ...not_minus
                neg  %ebx

                movb 1(%edx), %al
                test %al,     %al
                jz   .loop_exit
            ...not_minus:

            # character is invalid
            movl 16(%ebp), %eax
            movl $'0',     (%eax)
            mov  $2,       %eax
            jmp  .return
        ..greater_than_zero:
            cmp  $'9',   %al
            jg   ..invalid_char

        sub  $'0',   %al

        mov  %edx,   %edi
        mul  %ecx
        add  %eax,   %ebx

        mov  $10,    %eax
        mul  %ecx
        mov  %eax,   %ecx

        mov  %edi,   %edx

        jmp  .loop
    .loop_exit:

    movl 16(%ebp), %eax
    movl %ebx,    (%eax)

    xor  %eax, %eax

.return:
    mov  %ebp, %esp
    pop  %edi
    pop  %ebx
    pop  %ebp
    ret
