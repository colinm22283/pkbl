#include <stdint.h>

#include <console/print_dec.h>

#include <keyboard/keyboard.h>
#include <keyboard/keycode.h>

volatile bool keyboard_awaiting_key = false;
volatile bool keyboard_shift_down = false;
volatile bool keyboard_ctrl_down = false;
keycode_t keyboard_pressed_key = 0;

void keyboard_key_down_handler(keycode_t key) {
    switch (key) {
        case KC_SHIFT: keyboard_shift_down = true; break;
        case KC_CTRL: keyboard_ctrl_down = true; break;
        default: break;
    }

    if (keyboard_awaiting_key) {
        keyboard_pressed_key = key;
        keyboard_awaiting_key = false;
    }
}
void keyboard_key_up_handler(keycode_t key) {
    switch (key) {
        case KC_SHIFT: keyboard_shift_down = false; break;
        case KC_CTRL: keyboard_ctrl_down = false; break;
        default: break;
    }
}