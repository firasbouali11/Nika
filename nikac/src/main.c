#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/lexer.h"
#include "core/parser.h"
#include "core/visitor.h"
#include "stdc.h"
#include "io/file_reader.h"
#include "utils/exception_utils.h"

static void print_parser_dfs(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->token)
        printf("%s ", node->token->value);
    print_parser_dfs(node->condition);
    print_parser_dfs(node->left);
    print_parser_dfs(node->right);
    if (node->body)
        for (int i = 0; i < node->body->size; i++)
            print_parser_dfs(node->body->array[i]);
    if (node->args)
        for (int i = 0; i < node->args->size; i++)
            print_parser_dfs(node->args->array[i]);
}
void print_parser(ASTNode *node)
{
    print_parser_dfs(node);
    puts("");
}

void print_cache(Map *cache)
{
    printf("Map(");
    for (int i = 0; i < cache->capacity; i++)
    {
        Pair *elt = cache->array[i];
        while (elt)
        {
            char *f = (char *)elt->first;
            Value *s = (Value *)elt->second;
            printf("%s -> Value(%d, %p), ", f, s->type, (s->data));
            elt = elt->next;
        }
    }
    puts("NIL -> NIL)");
}

void print_functions(Map *functions)
{
    for (int i = 0; i < functions->capacity; i++)
    {
        Pair *p = functions->array[i];
        while (p)
        {
            char *f = (char *)p->first;
            ASTNode *s = (ASTNode *)p->second;
            printf("%s -> ", f);
            print_parser(s);
            p = p->next;
        }
    }
}

void print_tokens(List *tokens)
{
    int j;
    for (j = 0; j < tokens->size - 1; j++)
    {
        Token *token = (Token *)list_get(tokens, j);
        printf("Token(%d, %s) -- ", token->type, token->value);
    }
    Token *token = (Token *)list_get(tokens, j);
    printf("Token(%d, %s)\n", token->type, token->value);
}

void check_args(int argc)
{
    if (argc == 1)
        throw_exception("No file was specified");
    if (argc > 2)
        throw_exception("Too many arguments");
}

int main(int argc, char const *argv[])
{
    check_args(argc);
    const char *filename = argv[1];
    char *code = read_file(filename);
    Set *keywords = set_new();
    char *special_chars = "+-*/()\" =;:{}<>,[]";
    int n = strlen(special_chars);
    for (int i = 0; i < n; i++)
        set_add(keywords, &special_chars[i], sizeof(char));
    Lexer *lexer = init_lexer(code, keywords);
    List *tokens = create_tokens(lexer);
    Map *cache = map_new();
    Map *functions = map_new();
    puts("######### tokens #########");
    print_tokens(tokens);
    Parser *parser = parser_init(tokens);
    List *ast_nodes = parse(parser);
    puts("######### ASTs #########");
    for (int i = 0; i < ast_nodes->size; i++)
    {
        ASTNode *node = (ASTNode *)list_get(ast_nodes, i);
        print_parser(node);
        if(node->name && node->body)
            map_add(functions, node->name, node, (strlen(node->name)+1) * sizeof(char));
        else 
            execute(node, cache, functions);
    }
    puts("######### Cache #########");
    print_cache(cache);
    print_functions(functions);
}