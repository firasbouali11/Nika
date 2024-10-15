#pragma once

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int length;
} LinkedList;

LinkedList *linkedlist_new();

void linkedlist_append(LinkedList *list, void *data);

void linkedlist_prepend(LinkedList *list, void *data);

void linkedlist_insert(LinkedList *list, int pos, void *data);

void* linkedlist_delete(LinkedList *list, int pos);

int linkedlist_search(LinkedList *list, void *t, int size);