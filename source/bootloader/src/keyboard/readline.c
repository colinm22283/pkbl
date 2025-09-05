#include <keyboard/readline.h>
#include <keyboard/getch.h>

#include <console/put.h>
#include <console/backspace.h>

void keyboard_readline(char * buffer, uint32_t max_size) {
    char c = keyboard_getch();
    uint32_t position = 0;
    while (c != '\n' && position < max_size - 1) {
        if (c == 8) {
            if (position > 0) {
                position--;
                console_backspace();
            }
        }
        else {
            buffer[position++] = c;
            console_put(c);
        }

        c = keyboard_getch();
    }
    buffer[position] = '\0';
}