#pragma once

typedef struct SetElt
{
    void *data;
    int size;
    struct SetElt *next;
} SetElt;

typedef struct
{
    SetElt **array;
    int size;
    int capacity;
} Set;

Set *set_new();

void set_add(Set *set, void *element, int elt_size);

int set_contains(Set *set, void *element, int size);

SetElt *set_delete(Set *set, void *element, int elt_size);