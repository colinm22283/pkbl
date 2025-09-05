.code32

.global int_null_pic1_handler_entry
int_null_pic1_handler_entry:
    push %eax
    push %ecx
    push %edx
    push %edi
    push %esi

    cld
    call int_null_pic1_handler

    pop %esi
    pop %edi
    pop %edx
    pop %ecx
    pop %eax

    iret

.global int_null_pic2_handler_entry
int_null_pic2_handler_entry:
    push %eax
    push %ecx
    push %edx
    push %edi
    push %esi

    cld
    call int_null_pic2_handler

    pop %esi
    pop %edi
    pop %edx
    pop %ecx
    pop %eax

    iret