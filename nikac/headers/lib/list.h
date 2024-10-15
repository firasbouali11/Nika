#pragma once

typedef struct
{
    void **array;
    int size;
    int capacity;
} List;

List *list_new();

void list_add(List *list, void * val);

void *list_get(List *list, int pos);

void* list_pop(List *list);

void* list_delete(List *list, int pos);

void list_insert(List *list, int pos, void *val);