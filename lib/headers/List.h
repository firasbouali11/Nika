#pragma once

typedef struct {
    void **array;
    int size;
    int capacity;
} List;


List * newList();

void addToList(List* list, void*, int size);

void* getFromList(List* list, int pos);

void popFromList(List* list);

void deleteFromList(List* list, int pos);

void mergeSort(int **arr, int l, int h);

void freeList(List * list);

void insertToList(List* list, int pos, void* val, int size);