#pragma once

#define INIT_CAPACITY 3000

typedef struct {
    int * arr;
    int capacity;
    int size;
} Heap;

Heap* createHeap();
void heapify(Heap* heap, int i);
int parent(int elt);
int left(int elt);
int right(int elt);
int getMin(Heap* heap);
void insert(Heap* heap, int elt);
int pop(Heap* heap);
