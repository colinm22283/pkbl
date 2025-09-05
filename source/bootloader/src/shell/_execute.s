.code32

.section .text
.global shell_execute
shell_execute:
    push %ebp
    mov  %esp, %ebp

    movl 8(%esp), %eax

    xor  %ecx,    %ecx
    movb (%eax),  %cl
    test %cl,     %cl
    jz   .return_good

    inc  %eax
    sub  $97,  %cl

    cmp  $26,  %cl
    jae  .return_not_found

    mov  $shell_command_tree_root, %edx
    movl (%edx, %ecx, 4),          %edx

    .loop:
        test %edx,       %edx
        jz   .return_not_found

        xor  %ecx,   %ecx
        movb (%eax), %cl
        test %cl, %cl
        jnz ..not_zero
            movl (%edx), %ecx
            test %ecx,   %ecx
            jz   .return_not_found

            push %ebx
            push %esi
            push %edi

            mov %esp,                %esi
            sub $4,                  %esi
            xor %edi,                %edi
            mov $shell_input_buffer, %eax
            ..parse_loop:
                push %eax
                inc  %edi

                ...loop:
                    movb (%eax), %bl
                    inc  %eax
                    test %bl,    %bl
                    jnz  ...loop

                movb (%eax), %bl
                test %bl,    %bl
                jnz  ..parse_loop

            mov  %esi, %eax
            mov  %esp, %ebx
            ..reverse_arguments_loop:
                movl (%esi), %ecx
                movl (%ebx), %eax
                movl %eax,   (%esi)
                movl %ecx,   (%ebx)

                sub $4, %esi
                add $4, %ebx

                cmp  %esi, %ebx
                jl   ..reverse_arguments_loop

            mov  %edi, %ebx
            push %esp
            push %edi
            call *(%edx)
            add  $8, %esp
            lea  (%esp, %edi, 4), %esp

            pop %edi
            pop %esi
            pop %ebx
            pop %ebp
            ret
        ..not_zero:

        inc %eax
        sub $96,  %cl

        cmp  $27,  %cl
        jae  .return_not_found
        test %cl,  %cl
        jz   .return_not_found

        movl (%edx, %ecx, 4), %edx

        jmp .loop

.return_good:
    mov $0, %eax

    pop %ebp
    ret
.return_not_found:
    mov $0xFFFFFFFF, %eax

    pop %ebp
    ret
