#include <gdt64.h>

__SECTION(".gdt64") gdt64_t gdt64 = {
    .null = GDT64_NULL_ENTRY,
    .code = {
        .base_upper = 0,
        .base_lower = 0,
        .limit_upper = 0xF,
        .limit_lower = 0xFFFF,
        .flags =
            GDT_FLAGS_GRANULARITY |
            GDT_FLAGS_LONG_MODE,
        .access =
            GDT_ACCESS_PRESENT |
            GDT_ACCESS_PRIVILEGE_LEVEL(0) |
            GDT_ACCESS_TYPE |
            GDT_ACCESS_EXECUTABLE |
            GDT_ACCESS_READ_WRITE |
            GDT_ACCESS_ACCESSED,
    },
    .data = {
        .base_upper = 0,
        .base_lower = 0,
        .limit_upper = 0xF,
        .limit_lower = 0xFFFF,
        .flags =
            GDT_FLAGS_GRANULARITY |
            GDT_FLAGS_LONG_MODE,
        .access =
            GDT_ACCESS_PRESENT |
            GDT_ACCESS_PRIVILEGE_LEVEL(0) |
            GDT_ACCESS_TYPE |
            GDT_ACCESS_READ_WRITE |
            GDT_ACCESS_ACCESSED,
    },
};

__SECTION(".gdt64_ptr") gdt64_ptr_t gdt64_ptr = DEFINE_GDT64_POINTER(gdt64);