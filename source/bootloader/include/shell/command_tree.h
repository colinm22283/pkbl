#pragma once

#include <stdint.h>
#include <stddef.h>

#include <defs.h>

typedef struct __PACKED shell_command_tree_s {
    uint32_t (* command_ptr)(uint32_t argc, const char ** argv);
    const struct __PACKED shell_command_tree_s * next_char[26];
} shell_command_tree_entry_t;

extern shell_command_tree_entry_t * shell_command_tree_root[26];

void shell_command_tree_init();