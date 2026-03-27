#include "include/stdlib.h"

#include <stdint.h>
#include <stddef.h>

#define MIN_ORDER 5
#define MAX_ORDER 14
#define HEAP_SIZE (1 << MAX_ORDER)

uint8_t heap[HEAP_SIZE];

#define BLOCK_SIZE(order) (1 << (order))
#define NUM_BLOCKS(order) HEAP_SIZE / BLOCK_SIZE(order)

typedef struct alloc_header {
    struct alloc_header* next;
    uint8_t order;
} alloc_header_t;

alloc_header_t* free_list[MAX_ORDER + 1];

#define HEADER_SIZE sizeof(alloc_header_t)

void init_allocator() {
    free_list[MAX_ORDER] = (void*)heap;
    free_list[MAX_ORDER]->next = NULL;
    free_list[MAX_ORDER]->order = MAX_ORDER;

    for (int order = 0; order < MAX_ORDER; order++) {
        free_list[order] = NULL;
    }
}

alloc_header_t* find_buddy(alloc_header_t* curr){
    size_t offset = (uint8_t*)curr - heap;
    size_t buddy_offset = offset ^ (1 << (curr->order));
    alloc_header_t* buddy = (alloc_header_t*)(buddy_offset + heap);

    alloc_header_t* temp = free_list[curr->order];
    if (temp == NULL) return NULL;

    if (temp == buddy) {
        free_list[curr->order] = temp->next;
        return buddy;
    }

    while (temp->next != NULL) {
        if (temp->next == buddy) {
            temp->next = temp->next->next;
            return buddy;
        }
        temp = temp->next;
    }
    return NULL;
}

void split(int order) {
    if (free_list[order] == NULL) return;
    if (order <= MIN_ORDER || order > MAX_ORDER) return;

    alloc_header_t* curr = free_list[order];
    free_list[order] = curr->next;

    int block_size_half = BLOCK_SIZE(order - 1);
    alloc_header_t* buddy = (alloc_header_t*)((uint8_t*)curr + block_size_half);

    curr->order = order - 1;
    buddy->order = order - 1;

    buddy->next = free_list[order - 1];
    free_list[order - 1] = buddy;

    curr->next = free_list[order - 1];
    free_list[order - 1] = curr;
}

alloc_header_t* join(alloc_header_t* curr) {
    int order = curr->order;
    if (order < MIN_ORDER || order > MAX_ORDER) return NULL;

    alloc_header_t* buddy = find_buddy(curr);
    if (!buddy) return NULL;

    size_t offset = (uint8_t*)curr - heap;
    if (offset & (1 << (curr->order))) {
        alloc_header_t* temp = curr;
        curr = buddy;
        buddy = temp;
    }
    curr->order++;
    return curr;
}

void* malloc(int size) {
    size = size + HEADER_SIZE;
    int order = MIN_ORDER;
    while (BLOCK_SIZE(order) < size) order++;

    int tempOrder = order;
    while (tempOrder <= MAX_ORDER && !free_list[tempOrder]) tempOrder++;
    if (tempOrder == MAX_ORDER + 1) return NULL;

    while (tempOrder > order) {
        split(tempOrder);
        tempOrder--;
    }

    alloc_header_t* curr = free_list[order];
    free_list[order] = curr->next;
    curr->order = order;

    void* allocated = (void*)((uint8_t*)curr + HEADER_SIZE);

    return allocated;
}

void free(void* allocated) {
    alloc_header_t* curr = (alloc_header_t*)((uint8_t*)allocated - HEADER_SIZE);
    alloc_header_t* next = NULL;

    while ((next = join(curr))) {
        curr = next;
    }
    curr->next = free_list[curr->order];
    free_list[curr->order] = curr;
}
