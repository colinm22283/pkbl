#pragma once

#include <stdint.h>

extern const char * const console_color_str_lut[16];

uint8_t console_color_str_lookup(const char * str);