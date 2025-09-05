#pragma once

#include <sys/gdt/gdt32.h>

typedef struct __PACKED {
    gdt32_entry_t null;
    gdt32_entry_t code;
    gdt32_entry_t data;
} gdt16_t;