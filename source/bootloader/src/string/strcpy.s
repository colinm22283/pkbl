.code32

.global strcpy
strcpy:
    movl 4(%esp), %edx
    movl 8(%esp), %ecx

    # ecx: dst
    # edx: src

    .loop:
        movb (%ecx), %al
        movb %al,    (%edx)

        inc  %edx
        inc  %ecx

        test %al,    %al
        jnz  .loop

    ret
