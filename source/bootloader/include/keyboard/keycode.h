#pragma once

#include <stdint.h>

enum {
    KC_BACKSPACE = 8,
    KC_UP_ARROW = 128,
    KC_DOWN_ARROW,
    KC_LEFT_ARROW,
    KC_RIGHT_ARROW,
    KC_SHIFT,
    KC_CTRL,
    KC_ALT,
};

typedef uint8_t keycode_t;