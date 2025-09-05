#include "defs.h"

#include <gdt16.h>

gdt16_t gdt16 = {
    .null = GDT32_NULL_ENTRY,
    .code = {
        .base_upper = 0,
        .base_lower = 0,
        .limit_upper = 0xF,
        .limit_lower = 0xFFFF,
        .flags =
            GDT_FLAGS_GRANULARITY,
        .access =
            GDT_ACCESS_PRESENT |
            GDT_ACCESS_PRIVILEGE_LEVEL(0) |
            GDT_ACCESS_TYPE |
            GDT_ACCESS_EXECUTABLE |
            GDT_ACCESS_READ_WRITE |
            GDT_ACCESS_ACCESSED,
    },
    .data = {
        .base_upper = 0x0,
        .base_lower = 0x0,
        .limit_upper = 0xF,
        .limit_lower = 0xFFFF,
        .flags =
            GDT_FLAGS_GRANULARITY,
        .access =
            GDT_ACCESS_PRESENT |
            GDT_ACCESS_PRIVILEGE_LEVEL(0) |
            GDT_ACCESS_TYPE |
            GDT_ACCESS_READ_WRITE |
            GDT_ACCESS_ACCESSED,
    },
};

const gdt32_ptr_t gdt16_ptr = DEFINE_GDT32_POINTER(gdt16);