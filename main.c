#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre */
    size_t bloc_size; /* taille du memory bloc en octets*/
    long magic_number; /* 0x0123456789ABCDEFL */
} HEADER;

HEADER *first_block;

long magicn = 0x0123456789ABCDEFL;
intptr_t Block_size = sizeof(HEADER) + sizeof(magicn);

HEADER * new_block(size_t size){
    HEADER *new;
    new = sbrk(Block_size + size);
    new->bloc_size = size;
    new->ptr_next = NULL;
    new->magic_number = magicn;
    long* m_n = ((void*) new + sizeof(HEADER) + size);
    *m_n = magicn;
    return new;
}

void * malloc_is(size_t size){
    HEADER * res;

    if(first_block == NULL){
        res = new_block(size);
    }

    else {
        HEADER *temp = first_block;
        HEADER *temp_prev;

        while (temp->bloc_size < size && temp->ptr_next != NULL) {
            temp_prev = temp;
            temp = temp->ptr_next;
        }

        if (temp->bloc_size < size ) {
            res = new_block(size);
        }

        else if (temp->bloc_size == size ) {
            if (temp == first_block) {
                first_block = NULL;
                res = temp;
            }

            else {
                temp_prev->ptr_next = temp->ptr_next;
                res = temp;
            }
        }

        else{
            HEADER *new;
            temp->bloc_size = temp->bloc_size - (size + Block_size);
            long* m_n = ((void*) temp + sizeof(HEADER) + temp->bloc_size);
            *m_n = magicn;

            new = (void*)temp + Block_size + temp->bloc_size  ;
            new->bloc_size = size;
            new->ptr_next = NULL;
            new->magic_number = magicn;
            res = new;
        }
    }

    return res + 1;
}

void free_is(void* block_para){
    HEADER * block = ((HEADER*) block_para) - 1;
    long* mn = block_para + block->bloc_size;
    if(magicn != *mn){
         printf("Overwritted block :,(  og : %ld , new : %ld \n", magicn, *mn);
         exit(1);
    }
    if(first_block == NULL){
        first_block = block;
    }
    else {
        block->ptr_next = first_block;
        first_block = block;
    }
}

int main() {
    printf("Hello, World!\n");
    int* test1 = malloc_is(10* sizeof(int));
    int* test2 = malloc_is(10* sizeof(int));
    /*test2[0] = 45;
    test1[0] = 0;
    test1[2] = 6;
    test1[9] = 9;
    test1[5] = 69;*/
    printf("test10 : %d, test1_2: %d, test1_9 : %d, test2_0 : %d\n", test1[0], test1[2], test1[9], test2[0]);
    free_is(test1);
    int* test3 = malloc_is(10* sizeof(int));
    free_is(test2);
    int* test4 = malloc_is(2* sizeof(int));
    int* test5 = malloc_is(2* sizeof(int)); // Ca marche
    0;
    return 0;
}
