#include <stdlib.h>
#include <stdio.h>
#include "lib/stack.h"

#define EMPTY_STACK_ERROR "Stack is empty"

Stack* stack_new(){
    Stack* st = malloc(sizeof(Stack));
    LinkedList* list = linkedlist_new();
    st->list = list;
    return st;
}

int stack_is_empty(Stack* st){
    return st->list->length == 0;
}

void stack_push(Stack *st, void* val){
    LinkedList* l = st->list;
    linkedlist_prepend(l, val);
}

void* stack_pop(Stack* st){
    if(stack_is_empty(st)){
        fprintf(stderr, EMPTY_STACK_ERROR);
        exit(EXIT_FAILURE);
    }
    void* res = stack_head(st);
    linkedlist_delete(st->list, 0);
    return res;
}

void* stack_head(Stack* st){
    return st->list->head->data;
}