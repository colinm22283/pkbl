#include <shell/input.h>

volatile uint8_t shell_position = 0;
char shell_input_buffer[256];
bool shell_ready_to_execute = false;
bool shell_enable_input_write_back = true;