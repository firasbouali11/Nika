#include <stdio.h>
#include <stdlib.h>

#include "utils/exception_utils.h"

void throw_exception(const char * s){
    fprintf(stderr, "Error: %s/n", s);
    exit(EXIT_FAILURE);
}