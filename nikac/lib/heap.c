#include <stdlib.h>
#include <stdio.h>
#include "lib/heap.h"
#include "lib/list.h"

#define INIT_CAPACITY 300

Heap *heap_new()
{
    Heap *heap = malloc(sizeof(Heap));
    heap->arr = list_new();
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

void* heap_get_min(Heap *heap)
{
    return list_get(heap->arr, 0);
}

static void swap(void **i, void **j)
{
    void* temp = *i;
    *i = *j;
    *j = temp;
}

void heapify(Heap *heap, int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    List *list = heap->arr;
    if (l < heap->size && (*(int *)list_get(list, l)) < (*(int *)list_get(list, smallest)))
        smallest = l;
    if (r < heap->size && (*(int *)list_get(list, l)) < (*(int *)list_get(list, smallest)))
        smallest = r;
    if (smallest != i)
    {
        void * s = list_get(list, i);
        void * t = list_get(list, smallest);
        swap(&s, &t);
        heapify(heap, smallest);
    }
}

void heap_insert(Heap *heap, void* elt)
{
    int i = heap->size;
    List *list = heap->arr;
    heap->size++;
    list->array[i] = elt;
    while (i != 0 && list_get(list, parent(i)) > list_get(list, i))
    {
        void * s = list_get(list, i);
        void * t = list_get(list, parent(i));
        swap(&t, &s);
        i = parent(i);
    }
}

void* heap_pop(Heap *heap)
{
    if(heap->size == 1){
        heap->size--;
        return list_get(heap->arr, 0);
    }
    List *list = heap->arr;
    void* root = list_get(list, 0);
    list->array[0] = list_get(list, heap->size - 1);
    heap->size--;
    heapify(heap, 0);
    return root;
}