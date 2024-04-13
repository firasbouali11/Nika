#include <stdio.h>
#include <stdlib.h>

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
    {
        innerPrintParser(node->body->array[i]);
    }
    
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
            else
                printf("%s -> Value(%d, %f), ", f, s->type, *(double *)s->data);

            p = p->next;
        }
    }
    puts(")");
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

int main(int argc, char const *argv[])
{
    if (argc == 0)
    {
        fprintf(stderr, "Error: No file specified");
        exit(EXIT_FAILURE);
    }
    const char *filename = argv[1];
    char *code = readFile(filename);
    Set *keywords = newSet();
    addToSet(keywords, &(char){'+'}, sizeof(char));
    addToSet(keywords, &(char){'-'}, sizeof(char));
    addToSet(keywords, &(char){'*'}, sizeof(char));
    addToSet(keywords, &(char){'/'}, sizeof(char));
    addToSet(keywords, &(char){'('}, sizeof(char));
    addToSet(keywords, &(char){')'}, sizeof(char));
    addToSet(keywords, &(char){'"'}, sizeof(char));
    addToSet(keywords, &(char){' '}, sizeof(char));
    addToSet(keywords, &(char){'='}, sizeof(char));
    addToSet(keywords, &(char){';'}, sizeof(char));
    addToSet(keywords, &(char){'{'}, sizeof(char));
    addToSet(keywords, &(char){'}'}, sizeof(char));
    addToSet(keywords, &(char){'>'}, sizeof(char));
    addToSet(keywords, &(char){'<'}, sizeof(char));
    Lexer *lexer = initLexer(code, keywords);
    List *l = createTokens(lexer);
    puts("############### TOKENS ################");
    printTokens(l);
    Map *cache = newMap();
    puts("############### PARSER ################");
    for (int i = 0; i < l->size; i++)
    {
        List *tokens = (List *)getFromList(l, i);
        Parser *parser = initParser(tokens);
        ASTNode *res = parse(parser);
        Value* val = execute(res, cache);
        printParser(res);
    }
    // printing cache
    puts("############### CACHE ################");
    printCache(cache);
    return 0;
}