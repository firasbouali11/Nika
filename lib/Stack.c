#include <stdlib.h>
#include <stdio.h>
#include "headers/Stack.h"

Stack* newtStack(){
    Stack* st = malloc(sizeof(Stack));
    LinkedList* list = newLinkedList();
    st->list = list;
    return st;
}

int stackEmpty(Stack* st){
    return st->list->length == 0;
}

void pushStack(Stack *st, void* val, int size){
    LinkedList* l = st->list;
    prependToLinkedList(l, val, size);
}

void* popStack(Stack* st){
    if(stackEmpty(st)){
        printf("Stack is empty!");
        exit(1);
    }
    void* res = headStack(st);
    deleteFromLinkedList(st->list, 0);
    return res;
}

void* headStack(Stack* st){
    return st->list->head->data;
}

void destroyStack(Stack* st){
    destroyLinkedList(st->list);
    free(st);
}
