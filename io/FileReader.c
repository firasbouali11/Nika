#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void checkNikaFile(const char *filename)
{
    int n = strlen(filename);
    char suffix[5];
    int j = 0;
    for (int i = n - 5; i < n; i++)
    {
        suffix[j++] = filename[i];
    }
    if(strncmp(".nika", suffix, 5 * sizeof(char))){
        fprintf(stderr, "Error: This is not a Nika file");
        exit(EXIT_FAILURE);
    }
}

char *readFile(const char *filename)
{
    checkNikaFile(filename);
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File could not be opened.");
        exit(EXIT_FAILURE);
    }
    char c;
    char *code = malloc(sizeof(char));
    code[0] = '\0';
    int size = 0;
    while ((c = fgetc(file)) != EOF)
    {
        code = realloc(code, (size + 2) * sizeof(char));
        code[size] = c;
        code[size + 1] = '\0';
        size++;
    }
    fclose(file);
    return code;
}
