#include <stdlib.h>
#include <stdio.h>

#include "lib/stack.h"
#include "lib/linkedlist.h"

#define EMPTY_STACK_ERROR "Stack is empty"

int stack_is_empty(LinkedList *st)
{
    return st || st->head || st->length == 0;
}

void stack_push(LinkedList *st, void *val)
{
    linkedlist_prepend(st, val);
}

void *stack_pop(LinkedList *st)
{
    void *res = stack_head(st);
    linkedlist_delete(st, 0);
    return res;
}

void *stack_head(LinkedList *st)
{
    if (stack_is_empty(st))
    {
        fprintf(stderr, EMPTY_STACK_ERROR);
        exit(EXIT_FAILURE);
    }
    return st->head->data;
}