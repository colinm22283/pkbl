#pragma once

#include <console/console.h>

static inline console_char_t * console_frame_buffer() { return CONSOLE_BASE_POINTER; }

static inline uint32_t console_width() { return CONSOLE_WIDTH; }
static inline uint32_t console_height() { return CONSOLE_HEIGHT; }
