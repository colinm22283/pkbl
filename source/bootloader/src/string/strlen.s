.code32

.global strlen
strlen:
    movl 4(%esp),  %edx

    # edx: string

    xor  %eax,   %eax

    movb (%edx), %cl

    .loop:
        test %cl,    %cl
        jz   .return

        inc  %eax
        inc  %edx

        movb (%edx), %cl
        jmp  .loop

.return:
    ret
