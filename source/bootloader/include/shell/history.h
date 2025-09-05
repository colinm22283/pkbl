#pragma once

#include <stdint.h>

#define SHELL_HISTORY_SIZE (16)

void shell_history_init();
void shell_history_push(const char * command);
void shell_history_get(uint32_t index, char * dest);