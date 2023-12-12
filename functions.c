//
// Created by franzozich on 12/12/23.
//
#include <stdio.h>
#include "header.h"

HEADER *first_block;

intptr_t Block_size = sizeof(HEADER) + sizeof(long);

HEADER * new_block(size_t size){
    HEADER *new;
    new = sbrk(Block_size + size);
    new->bloc_size = size;
    new->ptr_next = NULL;
    new->ptr_prev = NULL;
    new->magic_number = MAGIC;
    long* m_n = (long*)((char*)new + Block_size + size - sizeof(long));
    *m_n = MAGIC;

    return new;
}




void * malloc_is(size_t size){
    HEADER * res;

    if(first_block == NULL){
        res = new_block(size);
    } else {
        HEADER *temp = first_block;
        HEADER *temp_prev = NULL;

        while (temp != NULL && temp->bloc_size < size) {
            temp_prev = temp;
            temp = temp->ptr_next;
        }

        if (temp == NULL) {
            res = new_block(size);
        } else if (temp->bloc_size == size) {
            if (temp_prev != NULL) {
                temp_prev->ptr_next = temp->ptr_next;
                if (temp->ptr_next != NULL) {
                    temp->ptr_next->ptr_prev = temp_prev;
                }
            } else {
                first_block = temp->ptr_next;
                if (temp->ptr_next != NULL) {
                    temp->ptr_next->ptr_prev = NULL;
                }
            }
            res = temp;
        } else {
            // Diviser le bloc libre
            HEADER *new_free_block = (HEADER*)((char*)temp + Block_size + size);
            new_free_block->bloc_size = temp->bloc_size - size - Block_size;
            new_free_block->ptr_next = temp->ptr_next;
            new_free_block->ptr_prev = temp_prev;
            new_free_block->magic_number = MAGIC;

            if (temp_prev != NULL) {
                temp_prev->ptr_next = new_free_block;
            } else {
                first_block = new_free_block;
            }

            if (new_free_block->ptr_next != NULL) {
                new_free_block->ptr_next->ptr_prev = new_free_block;
            }

            temp->bloc_size = size;
            res = temp;
        }
    }

    return ((char*)res) + sizeof(HEADER);
}


void merge_free_blocks(HEADER* block) {
    // Parcourir la liste pour fusionner les blocs adjacents
    HEADER *current = block;
    while (current != NULL) {
        HEADER *next = current->ptr_next;
        if (next != NULL && (char*)current + current->bloc_size + Block_size == (char*)next) {
            // Fusionner les blocs
            current->bloc_size += next->bloc_size + Block_size;
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
    HEADER *block = ((HEADER*) block_para) - 1;
    long* mn = (long*)((char*)block + Block_size + block->bloc_size - sizeof(long));

    if(MAGIC != *mn){
        printf("Bloc écrasé :,(  original : %ld , nouveau : %ld \n", MAGIC, *mn);
        exit(1);
    }

    block->ptr_next = first_block;
    if (first_block != NULL) {
        first_block->ptr_prev = block;
    }
    block->ptr_prev = NULL;
    first_block = block;

    merge_free_blocks(block);
}

