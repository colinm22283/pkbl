.code32

.global int_keyboard_handler_entry
int_keyboard_handler_entry:
    push %eax
    push %ecx
    push %edx
    push %edi
    push %esi

    cld
    call int_keyboard_handler

    pop %esi
    pop %edi
    pop %edx
    pop %ecx
    pop %eax

    iret