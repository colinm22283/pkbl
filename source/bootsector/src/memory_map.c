#include "sys/memory_map/memory_map.h"

#include "defs.h"

__SECTION(".memory_map")      memory_map_entry_t memory_map[(0x200 - 4) / sizeof(memory_map_entry_t)];
__SECTION(".memory_map_size") uint32_t           memory_map_size;