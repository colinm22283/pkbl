#include <stdbool.h>

#include <keyboard/getch.h>
#include <keyboard/keyboard.h>

#include <sys/asm/hlt.h>

char keyboard_getch() {
    keyboard_awaiting_key = true;
    while (keyboard_awaiting_key) hlt();
    return (char) keyboard_pressed_key;
}