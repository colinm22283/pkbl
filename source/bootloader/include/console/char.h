#pragma once

#include <stdint.h>

#include <defs.h>

#include <console/color.h>

typedef struct __PACKED {
    char ch;
    console_color_t color;
} console_char_t;