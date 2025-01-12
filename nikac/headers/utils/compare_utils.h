#pragma once
#include "lib/boolean.h"

bool equal(void *a, void *b, int size, int (*func)(void *, void *));

bool gt(void *a, void *b, int size, int (*func)(void *, void *));

bool gte(void *a, void *b, int size, int (*func)(void *, void *));

bool lt(void *a, void *b, int size, int (*func)(void *, void *));

bool lte(void *a, void *b, int size, int (*func)(void *, void *));