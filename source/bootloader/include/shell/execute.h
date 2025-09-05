#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <defs.h>

#define SHELL_MAX_ARGS (128)

enum {
    SHELL_EXECUTE_COMMAND_NOT_FOUND = 0xFFFFFFFF,
};

uint32_t shell_execute(char * command);