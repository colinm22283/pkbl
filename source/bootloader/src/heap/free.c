#include <stddef.h>
#include <stdbool.h>

#include <heap/free.h>
#include <heap/heap.h>

void heap_free(void * alloc) {
    heap_tag_t * tag = (heap_tag_t *) ((char *) alloc - sizeof(heap_tag_t));

    if (tag->prev_size != 0) {
        heap_tag_t * prev_tag = (heap_tag_t *) ((char *) tag - tag->prev_size - sizeof(heap_tag_t));
        if (!prev_tag->allocated) {
            tag = prev_tag;

            tag->next_size += tag->next_size + sizeof(heap_tag_t);
        }
    }

    if (tag->next_size != 0) {
        heap_tag_t * next_tag = (heap_tag_t *) ((char *) alloc + tag->next_size);
        if (next_tag != NULL && !next_tag->allocated) {
            tag->next_size += next_tag->next_size + sizeof(heap_tag_t);
        }
    }

    tag->allocated = false;
    ((heap_tag_t *) ((char *) tag + tag->next_size + sizeof(heap_tag_t)))->prev_size = tag->next_size;
}