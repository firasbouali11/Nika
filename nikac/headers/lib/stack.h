#pragma once

#include "linkedlist.h"

typedef struct
{
    LinkedList *list;
} Stack;

Stack *stack_new();

int stack_is_empty(Stack *st);

void stack_push(Stack *st, void *val);

void *stack_pop(Stack *st);

void *stack_head(Stack *st);