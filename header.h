#include <unistd.h>
#include <stdlib.h>
#ifndef LAB2_OS_HEADER_H
#define LAB2_OS_HEADER_H

#endif //LAB2_OS_HEADER_H

#define MAGIC 0x0123456789ABCDEFL

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre */
    struct HEADER_TAG *ptr_prev;
    size_t bloc_size; /* taille du memory bloc en octets*/
    long magic_number; /* 0x0123456789ABCDEFL */
} HEADER;

HEADER * create_new_block(size_t size);
void * malloc_is(size_t size);
void free_is(void* block_para);
void merge_free_blocks(HEADER* block);