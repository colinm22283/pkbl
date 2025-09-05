#pragma once

#include <stdint.h>

void pic_set_mask(unsigned char irq_line);
void pic_clear_mask(unsigned char irq_line);
void pic_mask_all();