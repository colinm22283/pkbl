.code32

.global strcmp
strcmp:
    push %ebp
    mov  %esp, %ebp

    movl 8(%esp),  %ecx
    movl 12(%esp), %edx

    xor  %eax,     %eax

    .loop:
        movb (%ecx), %al
        movb (%edx), %ah

        inc  %ecx
        inc  %edx

        cmp  %ah,    %al
        jnz  .exit_loop

        # test for both zero
        test %al,    %al
        jnz  ..lnz
            test %ah, %ah
            jz   .return
        ..lnz:

        # test for one zero
        test %al,    %ah
        jnz   .loop



    .exit_loop:

    sub %ah, %al
    xor %ah, %ah

    .return:

    pop %ebp
    ret
