#include <stdlib.h>
#include <string.h>

#include "utils/type_utils.h"

int *Int(int i)
{
    int *a = malloc(sizeof(int));
    *a = i;
    return a;
}

long *Long(long l)
{
    long *a = malloc(sizeof(long));
    *a = l;
    return a;
}

float *Float(float f)
{
    float *a = malloc(sizeof(float));
    *a = f;
    return a;
}

double *Double(double d)
{
    double *a = malloc(sizeof(double));
    *a = d;
    return a;
}

char *String(char *s)
{
    char *res = malloc((strlen(s) + 1) * sizeof(char));
    strcpy(res, s);
    return res;
}