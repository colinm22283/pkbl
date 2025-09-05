#include <commands/lsmem.h>

#include <console/print.h>
#include <console/print_hex.h>
#include <console/print_dec.h>
#include <console/newline.h>
#include <console/put.h>

#include <string/stoi.h>

#include <memory_map.h>

const char * type_lut[] = {
    "",
    "USABLE",
    "RESERVED",
    "ACPI RECLAIMABLE",
    "ACPI NVS",
    "BAD",
};

__CDECL uint32_t command_lsmem(uint32_t argc, const char ** argv) {
    if (argc == 1 || argc > 3) {
        console_print("lsmem expects 1-2 arguments\n");
        return 1;
    }

    int32_t temp;
    uint32_t start_index = 0, end_index = 0;

    if (stoi(&temp, argv[1])) {
        console_print("Invalid number for argument ");
        console_put('1');
        console_newline();
        return 2;
    }
    start_index = temp;

    switch (argc) {
        case 2: end_index = start_index; break;

        case 3: {
            if (stoi(&temp, argv[2])) {
                console_print("Invalid number for argument ");
                console_put('2');
                console_newline();
                return 3;
            }
            end_index = temp;
        } break;

        default: break;
    }

    if (start_index > end_index) {
        console_print("Start index must be less than or equal to the end\n");
        return 4;
    }

    if (start_index >= memory_map_size) {
        console_print("Start");
        console_print(" index is greater than the final index of memory entries (");
        console_print_dec(memory_map_size - 1);
        console_print(")\n");
        return 5;
    }
    if (end_index >= memory_map_size) {
        console_print("End");
        console_print(" index is greater than the final index of memory entries (");
        console_print_dec(memory_map_size - 1);
        console_print(")\n");
        return 5;
    }

    for (; start_index <= end_index; start_index++) {
        console_print("SECTOR ");
        console_print_dec(start_index);

        console_print("\n    START: 0x");
        console_print_hex(memory_map[start_index].base >> 32);
        console_print_hex(memory_map[start_index].base);

        console_print("\n    LENGTH: 0x");
        console_print_hex(memory_map[start_index].length >> 32);
        console_print_hex(memory_map[start_index].length);

        console_print("\n    END: 0x");
        console_print_hex((memory_map[start_index].base + memory_map[start_index].length - 1) >> 32);
        console_print_hex(memory_map[start_index].base + memory_map[start_index].length - 1);

        console_print("\n    TYPE: 0x");
        console_print_hex(memory_map[start_index].type);
        console_print(" (");
        console_print(type_lut[memory_map[start_index].type]);
        console_print(")\n");
    }

    return 0;
}