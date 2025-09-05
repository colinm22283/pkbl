.code32

.global int_timer_handler_entry
int_timer_handler_entry:
    push %eax
    push %ecx
    push %edx
    push %edi
    push %esi

    cld
    call int_timer_handler

    pop %esi
    pop %edi
    pop %edx
    pop %ecx
    pop %eax

    iret
