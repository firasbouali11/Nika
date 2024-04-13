#pragma once

#include "LinkedList.h"

typedef struct{
    LinkedList * list;
}Stack;

Stack* newtStack();

int stackEmpty(Stack* st);

void pushStack(Stack *st, void* val, int size);

void* popStack(Stack* st);

void* headStack(Stack* st);

void destroyStack(Stack* st);