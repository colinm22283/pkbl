#include <console/console.h>

console_char_t * console_output_ptr = CONSOLE_BASE_POINTER;
console_char_t * console_newline_ptr = CONSOLE_BASE_POINTER + CONSOLE_WIDTH;

console_char_t console_buffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];
unsigned char console_position = 0;