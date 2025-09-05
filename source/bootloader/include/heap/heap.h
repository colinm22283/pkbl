#pragma once

#include <stdint.h>

#include <defs.h>

typedef struct __PACKED {
    uint8_t allocated;
    uint32_t prev_size;
    uint32_t next_size;
} heap_tag_t;

extern heap_tag_t * heap_base;