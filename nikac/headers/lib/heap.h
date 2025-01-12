#pragma once

#include "list.h"

typedef struct
{
    List *arr;
    int capacity;
    int size;
} Heap;

Heap *heap_new();

void heapify(Heap *heap, int i);

int parent(int elt);

int left(int elt);

int right(int elt);

void *heap_get_min(Heap *heap);

void heap_insert(Heap *heap, void *elt);

void *heap_pop(Heap *heap);