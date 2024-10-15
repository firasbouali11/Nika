#include <stdlib.h>
#include <string.h>

#include "utils/string_utils.h"

char *char_to_string(char c)
{
    char *string = malloc(2 * sizeof(char));
    string[0] = c;
    string[1] = '\0';
    return string;
}

char *create_string(char *s, int size)
{
    char *string = malloc((size + 1) * sizeof(char));
    strcpy(string, s);
    return string;
}