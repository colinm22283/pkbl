#include <stdbool.h>

#include <interrupt/keyboard_lut.h>

#include <shell/keyboard_handler.h>

#include <console/print_dec.h>
#include <console/print.h>
#include <console/print_dec.h>
#include <console/newline.h>

#include <keyboard/keyboard.h>
#include <keyboard/keycode.h>

#include <sys/asm/out.h>
#include <sys/asm/in.h>
#include <sys/ports.h>

#include <console/print_dec.h>

bool shift_down = false;
bool in_extended = false;

void int_keyboard_handler() {
    unsigned char raw_char = (unsigned char) inb(0x60);

//    console_print_dec(raw_char);
//    console_newline();

    if (raw_char == 0xE0) {
        in_extended = true;
    }
    else {
        bool released = raw_char >= 0x80;

        if (released) raw_char -= 0x80;

        if (in_extended) {
            in_extended = false;

            if (released) {
                switch (raw_char) {
                    case 72: {
                        keyboard_key_up_handler(KC_UP_ARROW);
                    } break;

                    case 75: {
                        keyboard_key_up_handler(KC_LEFT_ARROW);
                    } break;

                    case 77: {
                        keyboard_key_up_handler(KC_RIGHT_ARROW);
                    } break;

                    case 80: {
                        keyboard_key_up_handler(KC_DOWN_ARROW);
                    } break;

                    default: break;
                }
            }
            else {
                switch (raw_char) {
                    case 72: {
                        keyboard_key_down_handler(KC_UP_ARROW);
                        shell_keyboard_key_down_handler(KC_UP_ARROW);
                    } break;

                    case 75: {
                        keyboard_key_down_handler(KC_LEFT_ARROW);
                        shell_keyboard_key_down_handler(KC_LEFT_ARROW);
                    } break;

                    case 77: {
                        keyboard_key_down_handler(KC_RIGHT_ARROW);
                        shell_keyboard_key_down_handler(KC_RIGHT_ARROW);
                    } break;

                    case 80: {
                        keyboard_key_down_handler(KC_DOWN_ARROW);
                        shell_keyboard_key_down_handler(KC_DOWN_ARROW);
                    } break;

                    default: break;
                }
            }
        }
        else {
            char ascii_char = keyboard_lut[raw_char];

            if (ascii_char == 9) {
                switch (raw_char) {
                    case 42:
                    case 54: {
                        if (released) {
                            shift_down = false;

                            keyboard_key_up_handler(KC_SHIFT);
                        }
                        else {
                            shift_down = true;

                            keyboard_key_down_handler(KC_SHIFT);
                            shell_keyboard_key_down_handler(KC_SHIFT);
                        }
                    } break;

                    case 29: {
                        if (released) {
                            keyboard_key_up_handler(KC_CTRL);
                        }
                        else {
                            keyboard_key_down_handler(KC_CTRL);
                            shell_keyboard_key_down_handler(KC_CTRL);
                        }
                    } break;

                    default:
                        break;
                }
            }
            else {
                if (shift_down) {
                    char shifted_ascii_char = keyboard_lut_shift[raw_char];
                    if (released) {
                        keyboard_key_up_handler(shifted_ascii_char);
                    }
                    else {
                        keyboard_key_down_handler(shifted_ascii_char);
                        shell_keyboard_key_down_handler(shifted_ascii_char);
                    }
                }
                else {
                    if (released) {
                        keyboard_key_up_handler(ascii_char);
                    }
                    else {
                        keyboard_key_down_handler(ascii_char);
                        shell_keyboard_key_down_handler(ascii_char);
                    }
                }
            }
        }
    }

    outb(PORT_PIC1_COMMAND, 0x20);
}