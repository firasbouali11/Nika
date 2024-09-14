#include <stdlib.h>
#include <stdio.h>
#include "headers/heap.h"

Heap *createHeap()
{
    int *arr = malloc(INIT_CAPACITY * sizeof(int));
    Heap *heap = malloc(sizeof(Heap));
    heap->arr = arr;
    heap->capacity = INIT_CAPACITY;
    heap->size = 0;
    return heap;
}

int parent(int i)
{
    return (i - 1) / 2;
}

int left(int i)
{
    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}

int getMin(Heap *heap)
{
    return heap->arr[0];
}

static void swap(int *i, int *j)
{
    int temp = *i;
    *i = *j;
    *j = temp;
}

void heapify(Heap *heap, int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    int *arr = heap->arr;
    if (l < heap->size && arr[l] < arr[smallest])
        smallest = l;
    if (r < heap->size && arr[r] < arr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&arr[i], &arr[smallest]);
        heapify(heap, smallest);
    }
}

void insert(Heap *heap, int elt)
{
    int i = heap->size;
    int *arr = heap->arr;
    heap->size++;
    arr[i] = elt;
    while (i != 0 && arr[parent(i)] > arr[i])
    {
        swap(&arr[parent(i)], &arr[i]);
        i = parent(i);
    }
}

int pop(Heap *heap)
{
    if(heap->size == 1){
        heap->size--;
        return heap->arr[0];
    }
    int *arr = heap->arr;
    int root = arr[0];
    arr[0] = arr[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return root;
}