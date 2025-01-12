#include <string.h>

#include "utils/compare_utils.h"
#include "lib/boolean.h"

bool equal(void *a, void *b, int size, int (*func)(void *, void *))
{
    if (func)
        return func(a, b);
    return memcmp(a, b, size) == 0;
}

bool gt(void *a, void *b, int size, int (*func)(void *, void *))
{
    if (func)
        return func(a, b);
    return memcmp(a, b, size) > 0;
}

bool lt(void *a, void *b, int size, int (*func)(void *, void *))
{
    if (func)
        return func(a, b);
    return memcmp(a, b, size) < 0;
}

bool gte(void *a, void *b, int size, int (*func)(void *, void *))
{
    if (func)
        return func(a, b);
    return memcmp(a, b, size) >= 0;
}

bool lte(void *a, void *b, int size, int (*func)(void *, void *))
{
    if (func)
        return func(a, b);
    return memcmp(a, b, size) <= 0;
}