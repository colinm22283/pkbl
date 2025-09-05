#include <console/update.h>
#include <console/console.h>

#include <memory/memcpy.h>

void console_update() {
    memcpy(CONSOLE_BASE_POINTER, console_buffer, CONSOLE_WIDTH * CONSOLE_HEIGHT * sizeof(console_char_t));
}