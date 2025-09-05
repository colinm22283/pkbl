.code32

.global int_fatal_handler_entry
int_fatal_handler_entry:
    cli

    mov $0xB8001,   %eax
    mov $(80 * 25), %ecx
    .clear_loop:
        movb $1, (%eax)

        add  $2, %eax

        loop .clear_loop

    mov $error_string_line_1, %eax
    mov $0xB8001,             %ebx
    mov $23,                  %ecx
    .print_loop_line_1:
        movb (%eax), %dl
        movb %dl,    (%ebx)

        inc  %eax
        add  $2,     %ebx

        loop .print_loop_line_1

    mov $error_string_line_2,  %eax
    mov $(0xB8000 + (2 * 80)), %ebx
    mov $23,                   %ecx
    .print_loop_line_2:
        movb (%eax), %dl
        movb %dl,    (%ebx)

        inc  %eax
        add  $2,     %ebx

        loop .print_loop_line_2

    .halt_loop:
        hlt
        jmp .halt_loop

error_string_line_1: .string "FATAL ERROR ENCOUNTERED"
error_string_line_2: .string "HALTING..."
