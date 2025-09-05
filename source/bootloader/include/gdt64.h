#pragma once

#include <sys/gdt/gdt64.h>
#include <sys/tss/tss64.h>

typedef struct __PACKED {
    gdt64_entry_t null;
    gdt64_entry_t code;
    gdt64_entry_t data;
    gdt64_entry_t tss;
} gdt64_t;

extern gdt64_t gdt64;

extern gdt64_ptr_t gdt64_ptr;