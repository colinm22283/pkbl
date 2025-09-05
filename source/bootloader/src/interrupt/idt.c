#include <interrupt/idt.h>

idt32_t idt;

idt_ptr_t idt_ptr = DEFINE_IDT_POINTER(idt);