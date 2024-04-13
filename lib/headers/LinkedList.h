#pragma once

typedef struct Node {
    void* data;
    struct Node * next;
} Node;

typedef struct {
    Node* head;
    Node * tail;
    int length;
} LinkedList;

LinkedList* newLinkedList();

void appendToLinkedList(LinkedList* list, void* data, int size);

void prependToLinkedList(LinkedList* list, void* data, int size);

void insertToLinkedList(LinkedList* list, int pos, void* data, int size);

void deleteFromLinkedList(LinkedList* list, int pos);

int searchInLinkedList(LinkedList* list, void* t, int size);

void printLinkedList(LinkedList * list);

void destroyLinkedList(LinkedList* list);