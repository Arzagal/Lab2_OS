
#include <stdio.h>
#include "header.h"

HEADER *first_block; // We create a chained list of the block available we already created inside the heap

intptr_t headNtail_size = sizeof(HEADER) + sizeof(long); //

HEADER * new_block(size_t size){ //Function called to increase the heap with a block of a certain size
    HEADER *new;
    new = sbrk(headNtail_size + size); //We call sbrk to increase the heap
    new->bloc_size = size;
    new->ptr_next = NULL;
    new->ptr_prev = NULL;
    new->magic_number = MAGIC; // Indicator we will use to ckeck the value at the tail
    long* m_n = (long*)((char*)new + headNtail_size + size - sizeof(long)); // We go at the tail
    *m_n = MAGIC; // At the end of the block there is a know value, that will be change if the user go too far

    return new;
}

void * malloc_is(size_t size){ //
    HEADER * res;

    if(first_block == NULL){ // If we dont have any available block
        res = new_block(size); //we create a new one with the value we want
    } else {
        HEADER *temp = first_block;
        HEADER *temp_prev = NULL;

        while (temp != NULL && temp->bloc_size < size) { // We itearate through the available block until we found one we can use
            temp_prev = temp;
            temp = temp->ptr_next;
        }

        if (temp == NULL) { // If there is no result
            res = new_block(size); // We create a new block
        } else if (temp->bloc_size == size) { // If the block has exactly the size we want
            if (temp_prev != NULL) { // If the block we found is the not first one, we remove the element from the list
                // We remove the block from the list of available blocks
                temp_prev->ptr_next = temp->ptr_next;
                if (temp->ptr_next != NULL) {
                    temp->ptr_next->ptr_prev = temp_prev;
                }
            }
            else { // If the block is the first one in the list
                first_block = temp->ptr_next;
                if (temp->ptr_next != NULL) {
                    temp->ptr_next->ptr_prev = NULL;
                }
            }
            res = temp;
        } else { //If the block found as a bigger size than needed
            // We will divide the block to create a block of the wanted size
            HEADER *new_free_block = (HEADER*)((char*)temp + headNtail_size + size); // We create a new block at the end of the one we found
            new_free_block->bloc_size = temp->bloc_size - size - headNtail_size;
            new_free_block->ptr_next = temp->ptr_next;
            new_free_block->ptr_prev = temp_prev;
            new_free_block->magic_number = MAGIC;

            // We put the new block in the list
            if (temp_prev != NULL) {
                temp_prev->ptr_next = new_free_block;
            } else {
                first_block = new_free_block;
            }

            if (new_free_block->ptr_next != NULL) {
                new_free_block->ptr_next->ptr_prev = new_free_block;
            }

            // We return the blovk, that now has the correct size
            temp->bloc_size = size;
            res = temp;
        }
    }

    return ((char*)res) + sizeof(HEADER); // We return a pointer to the data, not the header of the block
}


void merge_free_blocks(HEADER* block) {
    // The goal of this function is to iterate through the list of available blocks.
    // If 2 blovk are next to each other in the heap, we fuse them in a single block
    HEADER *current = block;
    while (current != NULL) {
        HEADER *next = current->ptr_next;
        if (next != NULL && (char*)current + current->bloc_size + headNtail_size == (char*)next) {
            // The block are touching in the heap, we fuse them
            current->bloc_size += next->bloc_size + headNtail_size;
            current->ptr_next = next->ptr_next;
            if (next->ptr_next != NULL) {
                next->ptr_next->ptr_prev = current;
            }
        } else {
            current = next;
        }
    }
}

void free_is(void* block_para){
    HEADER *block = ((HEADER*) block_para) - 1; // We had to get the Block header
    long* mn = (long*)((char*)block + headNtail_size + block->bloc_size - sizeof(long));

    if(MAGIC != *mn){
        // We check if the value has changed, i.e. if the user
        printf("ERROR : you have overwrite.  original : %ld , new : %ld \n", MAGIC, *mn);
        exit(1);
    }

    // We put the block at the beginning
    block->ptr_next = first_block;
    if (first_block != NULL) {
        first_block->ptr_prev = block;
    }
    block->ptr_prev = NULL;
    first_block = block;

    merge_free_blocks(block); // We fuse the blocks
}

