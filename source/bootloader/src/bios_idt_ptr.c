#include <sys/idt/idt_ptr.h>

idt_ptr_t bios_idt_ptr = {
    .offset = 0,
    .size = 0x3FF,
};