#pragma once

#include <console/console.h>
#include <console/newline.h>

static inline void inline_put(char c) {
    if (c == '\n') {
        console_newline();
    }
    else {
        console_buffer[CONSOLE_HEIGHT - 1][console_position].ch = c;
        console_position++;

        if (console_position == CONSOLE_WIDTH) {
            console_newline();
            console_buffer[CONSOLE_HEIGHT - 1][console_position].ch = ' ';
            console_position++;
        }
    }
}

static inline void inline_put_color(char c, console_color_t color) {
    if (c == '\n') {
        console_newline();
    }
    else {
        console_buffer[CONSOLE_HEIGHT - 1][console_position].ch = c;
        console_buffer[CONSOLE_HEIGHT - 1][console_position].color = color;
        console_position++;

        if (console_position == CONSOLE_WIDTH) {
            console_newline();
            console_buffer[CONSOLE_HEIGHT - 1][console_position].ch = ' ';
            console_position++;
        }
    }
}