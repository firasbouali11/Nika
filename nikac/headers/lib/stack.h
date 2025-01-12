#pragma once

#include "linkedlist.h"

int stack_is_empty(LinkedList *st);

void stack_push(LinkedList *st, void *val);

void *stack_pop(LinkedList *st);

void *stack_head(LinkedList *st);