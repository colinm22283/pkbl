.code32

.global command_fibonacci
command_fibonacci:
    push %ebp
    mov  %esp, %ebp

    push $.fibonacci_string
    call console_print
    add  $4,   %esp

    sub  $8,   %esp

    movl $1,   (%esp)
    movl $1,   4(%esp)

    .loop:
        call console_newline
        call console_print_dec

        movl (%esp),  %ecx
        movl 4(%esp), %edx

        mov  %edx,    %eax
        add  %ecx,    %edx
        mov  %eax,    %ecx

        movl %ecx, (%esp)
        movl %edx, 4(%esp)

        jno  .loop

    call console_newline

    add  $8,   %esp

    xor  %eax, %eax

    pop  %ebp
    ret

    .fibonacci_string: .asciz "[Fibonacci Number Generator]"
