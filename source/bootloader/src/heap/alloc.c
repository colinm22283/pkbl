#include <stddef.h>
#include <stdbool.h>

#include <heap/alloc.h>
#include <heap/heap.h>

void * heap_alloc(uint32_t size) {
    heap_tag_t * tag = heap_base;

    while (true) {
        if (tag->next_size == 0) return NULL;

        if (!tag->allocated && tag->next_size >= size) break;

        tag = (heap_tag_t *) (((char *) tag) + tag->next_size + sizeof(heap_tag_t));
    }

    if (tag->next_size - sizeof(heap_tag_t) > size) {
        tag->allocated = true;
        tag->next_size = size;

        heap_tag_t * mid_tag = (heap_tag_t *) (((char *) tag) + sizeof(heap_tag_t) + size);

        mid_tag->allocated = false;
        mid_tag->prev_size = size;
        mid_tag->next_size = tag->next_size - size - sizeof(heap_tag_t);

        heap_tag_t * end_tag = (heap_tag_t *) (((char *) tag) + sizeof(heap_tag_t) + tag->next_size);
        end_tag->prev_size = tag->next_size - size - sizeof(heap_tag_t);

        return ((char *) tag) + sizeof(heap_tag_t);
    }
    else {
        tag->allocated = true;

        return (char *) tag + sizeof(heap_tag_t);
    }
}