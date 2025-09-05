#include <stdbool.h>

#include <memory_map.h>

#include <console/print.h>
#include <console/print_hex.h>

__SECTION(".memory_map")           volatile memory_map_entry_t memory_map[(0x200 - 4) / sizeof(memory_map_entry_t)];
__SECTION(".memory_map_size")      volatile const uint32_t     memory_map_size;
__SECTION(".kernel_region_base")   volatile uint64_t           kernel_region_base;
__SECTION(".kernel_region_length") volatile uint64_t           kernel_region_length;

void memory_map_sort() { // called in bootloader_entry.s
    volatile memory_map_entry_t * base_pointer = memory_map;
    volatile memory_map_entry_t * const top_pointer = memory_map + memory_map_size - 1;

    for (; base_pointer <= top_pointer; base_pointer++) {
        for (volatile memory_map_entry_t * current = base_pointer; current <= base_pointer; current++) {
            if (current->base < base_pointer->base) {
                memory_map_entry_t temp = *current;
                *current = *base_pointer;
                *base_pointer = temp;
            }
        }
    }
}

primary_memory_region_t get_primary_memory_region() {
    primary_memory_region_t best_region = { .base = 0, .length = 0, };

    for (uint32_t i = 0; i < memory_map_size; i++) {
        switch (memory_map[i].type) {
            case MEMORY_MAP_TYPE_USABLE: case MEMORY_MAP_TYPE_ACPI_RECLAIMABLE: {
                primary_memory_region_t new_region = {
                    .base = memory_map[i].base,
                    .length = memory_map[i].length,
                };

                for (; i < memory_map_size - 1; i++) {
                    if (memory_map[i + 1].type == MEMORY_MAP_TYPE_USABLE || memory_map[i + 1].type == MEMORY_MAP_TYPE_ACPI_RECLAIMABLE) {
                        new_region.length += memory_map[i + 1].length;
                    }
                    else break;
                }

                if (new_region.length >= best_region.length) best_region = new_region;
            } break;

            default: break;
        }
    }

    best_region.base   = 0x100000;
    best_region.length = 0x1000000;

    return best_region;
}