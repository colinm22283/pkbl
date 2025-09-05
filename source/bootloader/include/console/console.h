#pragma once

#include <defs.h>

#include <console/char.h>

#define CONSOLE_BASE_POINTER ((console_char_t *) 0xB8000)
#define CONSOLE_WIDTH        (80)
#define CONSOLE_HEIGHT       (25)

extern console_char_t console_buffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];
extern unsigned char console_position;