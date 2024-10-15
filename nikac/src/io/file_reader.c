#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int check_nika_file(const char *filename)
{
    int n = strlen(filename);
    const char *suffix = ".nika";
    int j = 0;
    for (int i = n - 5; i < n; i++)
        if (suffix[j++] != filename[i])
            return 0;
    return 1;
}

static char *get_code_from_params(const char *param)
{
    int size = strlen(param) + 1;
    char *code = malloc(size * sizeof(char));
    strcpy(code, param);
    return code;
}

static char *get_code_from_file(const char *filename)
{
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

char *read_file(const char *filename)
{
    int check = check_nika_file(filename);
    if (!check)
        return get_code_from_params(filename);
    return get_code_from_file(filename);
}
