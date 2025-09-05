#include <stddef.h>
#include <stdbool.h>

#include <heap/init.h>
#include <heap/heap.h>

#include <memory_map.h>

void heap_init(void) {
    primary_memory_region_t primary_region = get_primary_memory_region();

    uint32_t start = primary_region.base;
    uint32_t end;

    if (primary_region.length > UINT32_MAX) end = UINT32_MAX;
    else end = primary_region.length;

    uint32_t size = end - start;

    heap_base = (heap_tag_t *) start;
    heap_tag_t * heap_tail = (heap_tag_t *) (end - sizeof(heap_tag_t));

    heap_base->allocated = false;
    heap_base->next_size = size - 2 * sizeof(heap_tag_t);
    heap_base->prev_size = 0;

    heap_tail->allocated = false;
    heap_tail->next_size = 0;
    heap_tail->prev_size = size - 2 * sizeof(heap_tag_t);
}