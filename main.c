#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/headers/Lexer.h"
#include "src/headers/Parser.h"
#include "src/headers/Visitor.h"
#include "lib/headers/Map.h"
#include "io/FileReader.h"

void innerPrintParser(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->token)
        printf("%s ", node->token->value);
    innerPrintParser(node->condition);
    innerPrintParser(node->left);
    innerPrintParser(node->right);
    for (int i = 0; i < node->body->size; i++)
        innerPrintParser(node->body->array[i]);
    for (int i = 0; i < node->args->size; i++)
        innerPrintParser(node->args->array[i]);
}
void printParser(ASTNode *node)
{
    innerPrintParser(node);
    puts("");
}

void printCache(Map *cache)
{
    printf("Map(");
    for (int i = 0; i < cache->capacity; i++)
    {
        Pair *p = cache->array[i];
        while (p)
        {
            char *f = (char *)p->first;
            Value *s = (Value *)p->second;
            if (s->type == TOKEN_STRING)
                printf("%s -> Value(%d, %s), ", f, s->type, (char *)s->data);
            else if (s->type == TOKEN_INT || s->type == TOKEN_BOOL)
                printf("%s -> Value(%d, %d), ", f, s->type, *(int *)s->data);
            else if (s->type == TOKEN_OSB)
            {
                List *list = (List *)s->data;
                printf("%s -> List(", f);
                for (int i = 0; i < list->size; i++)
                {
                    Value *val = (Value *)getFromList(list, i); // assuming the values are integers
                    if (i == list->size - 1)
                        printf("%d", *(int *)val->data);
                    else
                        printf("%d, ", *(int *)val->data);
                }
                printf("), ");
            }
            else
                printf("%s -> Value(%d, %f), ", f, s->type, *(double *)s->data);

            p = p->next;
        }
    }
    puts("NIL -> NIL)");
}

void printFunctions(Map *functions)
{
    for (int i = 0; i < functions->capacity; i++)
    {
        Pair *p = functions->array[i];
        while (p)
        {
            char *f = (char *)p->first;
            ASTNode *s = (ASTNode *)p->second;
            printf("%s -> ", f);
            printParser(s);
            p = p->next;
        }
    }
}

void printTokens(List *l)
{
    for (int i = 0; i < l->size; i++)
    {
        List *tokens = (List *)getFromList(l, i);
        puts("------------------------");
        for (int j = 0; j < tokens->size; j++)
        {
            Token *token = (Token *)getFromList(tokens, j);
            printf("Token(%d, %s)\n", token->type, token->value);
        }
    }
}

void checkArgs(int argc)
{
    if (argc == 1)
    {
        fprintf(stderr, "Error: no file was specified\n");
        exit(EXIT_FAILURE);
    }

    if (argc > 2)
    {
        fprintf(stderr, "Error: too many arguments\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    checkArgs(argc);
    const char *filename = argv[1];
    char *code = readFile(filename);
    Set *keywords = newSet();
    char *keywords_c = "+-*/()\" =;:{}<>,[]";
    for (int i = 0; i < strlen(keywords_c); i++)
        addToSet(keywords, &keywords_c[i], sizeof(char));
    Lexer *lexer = initLexer(code, keywords);
    List *l = createTokens(lexer);
    puts("############### TOKENS ################");
    printTokens(l);
    Map *cache = newMap();
    Map *functions = newMap();
    puts("############### PARSER ################");
    for (int i = 0; i < l->size; i++)
    {
        List *tokens = (List *)getFromList(l, i);
        Parser *parser = initParser(tokens);
        ASTNode *res = parse(parser);
        printParser(res);
        if (res->name && res->body)
            addToMap(functions, res->name, res, (strlen(res->name) + 1) * sizeof(char));
        else
            execute(res, cache, functions);
    }
    puts("############### CACHE ################");
    printCache(cache);
    puts("############### FUNCTIONS ################");
    printFunctions(functions);
    return 0;
}