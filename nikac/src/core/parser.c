#include <stdlib.h>
#include <stdio.h>

#include "core/lexer.h"
#include "core/parser.h"
#include "utils/exception_utils.h"

static ASTNode *ast_init(Token *token, ASTNode *left, ASTNode *right)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->token = token;
    node->left = left;
    node->right = right;
    node->condition = NULL;
    node->name = NULL;
    node->body = NULL;
    node->args = NULL;
    return node;
}

static void advance_parser(Parser *parser)
{
    parser->i++;
    if (parser->i < parser->tokens->size)
    {
        parser->current_token = (Token *)list_get(parser->tokens, parser->i);
    }
}

static int check_next_token(Parser *parser, int type)
{
    Token *next_token = (Token *)list_get(parser->tokens, parser->i + 1);
    return (int)next_token->type == type;
}

static void append_else_to_elf(ASTNode *elf_node, ASTNode *else_node)
{
    if (!elf_node->left)
    {
        elf_node->left = else_node;
        return;
    }
    append_else_to_elf(elf_node->left, else_node);
}

Parser *parser_init(List *tokens)
{
    Parser *parser = malloc(sizeof(Parser));
    parser->current_token = (void *)0;
    parser->i = -1;
    parser->tokens = tokens;
    return parser;
}

ASTNode *assign_expr(Parser *parser)
{
    if (parser->current_token && parser->current_token->type != TOKEN_VAR)
    {
        fprintf(stderr, "Error: Expected a variable\n");
        exit(EXIT_FAILURE);
    }
    Token *var_name = parser->current_token;
    advance_parser(parser);
    if (parser->current_token && parser->current_token->type != TOKEN_EQUAL)
    {
        fprintf(stderr, "Error: Expected an '='\n");
        exit(EXIT_FAILURE);
    }
    ASTNode *var_name_node = ast_init(var_name, NULL, NULL);
    Token *eq = parser->current_token;
    advance_parser(parser);
    ASTNode *expression = expr(parser);
    return ast_init(eq, var_name_node, expression);
}

List *block(Parser *parser)
{
    List *block_list = list_new();
    if (parser->current_token && parser->current_token->type == TOKEN_OB)
    {
        advance_parser(parser);
        while (parser->current_token && parser->current_token->type != TOKEN_CB)
        {
            ASTNode *node = expr(parser);
            if (node)
                list_add(block_list, node);
        }
        advance_parser(parser);
    }
    return block_list;
}

List *program(Parser *parser)
{
    List *program = list_new();
    while (parser->current_token && parser->current_token->type != TOKEN_EOF)
    {
        ASTNode *node = expr(parser);
        if (node)
            list_add(program, node);
    }
    return program;
}

ASTNode *handle_function_definition(Parser *parser)
{
    advance_parser(parser);
    if (parser->current_token && parser->current_token->type != TOKEN_VAR)
        throw_exception("Expected a variable.");

    ASTNode *node = ast_init(parser->current_token, NULL, NULL);
    node->name = parser->current_token->value;
    advance_parser(parser);

    if (parser->current_token && parser->current_token->type != TOKEN_LP)
        throw_exception("Expected a '('.");

    advance_parser(parser);
    node->args = list_new();
    while (parser->current_token && parser->current_token->type != TOKEN_RP)
    {
        if (parser->current_token->type == TOKEN_COMMA)
        {
            advance_parser(parser);
            continue;
        }
        advance_parser(parser);
        ASTNode *arg = expr(parser);
        list_add(node->args, arg);
    }
    advance_parser(parser);
    node->body = block(parser);
    return node;
}

ASTNode *handle_print_function(Parser *parser)
{
    Token *print_token = parser->current_token;
    advance_parser(parser);
    ASTNode *var_to_print = expr(parser);
    ASTNode *node = ast_init(print_token, var_to_print, NULL);
    return node;
}

ASTNode *handle_variable(Parser *parser)
{
    Token *var_name = parser->current_token;
    ASTNode *var_name_node = ast_init(var_name, NULL, NULL);
    advance_parser(parser);
    Token *eq = parser->current_token;
    advance_parser(parser);
    ASTNode *expression = expr(parser);
    return ast_init(eq, var_name_node, expression);
}

ASTNode *expr(Parser *parser)
{
    if (parser->current_token && parser->current_token->type == TOKEN_PRINT_FUNCTION)
        return handle_print_function(parser);

    if (parser->current_token && parser->current_token->type == TOKEN_TYPE_FUNCTION)
        return handle_function_definition(parser);

    if (parser->current_token && parser->current_token->type == TOKEN_SEMICOLON)
    {
        advance_parser(parser);
        return NULL;
    }
    if (
        parser->current_token &&
        (parser->current_token->type == TOKEN_TYPE_INT ||
         parser->current_token->type == TOKEN_TYPE_FLOAT ||
         parser->current_token->type == TOKEN_TYPE_STRING ||
         parser->current_token->type == TOKEN_TYPE_LIST ||
         parser->current_token->type == TOKEN_TYPE_BOOL))
    {
        advance_parser(parser);
        return assign_expr(parser);
    }
    if (parser->current_token && parser->current_token->type == TOKEN_VAR)
    {
        if (check_next_token(parser, TOKEN_EQUAL) || check_next_token(parser, TOKEN_COLON))
            return handle_variable(parser);
    }
    return logic_expr(parser);
}

ASTNode *logic_expr(Parser *parser)
{
    ASTNode *left = comp_expr(parser);
    while (
        parser->current_token &&
        (parser->current_token->type == TOKEN_AND ||
         parser->current_token->type == TOKEN_OR ||
         parser->current_token->type == TOKEN_TO))
    {
        Token *current = parser->current_token;
        advance_parser(parser);
        ASTNode *right = comp_expr(parser);
        left = ast_init(current, left, right);
    }
    return left;
}

ASTNode *comp_expr(Parser *parser)
{
    ASTNode *left = arith_expr(parser);
    while (
        parser->current_token &&
        (parser->current_token->type == TOKEN_EE ||
         parser->current_token->type == TOKEN_GT ||
         parser->current_token->type == TOKEN_LT ||
         parser->current_token->type == TOKEN_GTE ||
         parser->current_token->type == TOKEN_LTE))
    {
        Token *current = parser->current_token;
        advance_parser(parser);
        ASTNode *right = arith_expr(parser);
        left = ast_init(current, left, right);
    }
    return left;
}

ASTNode *arith_expr(Parser *parser)
{
    ASTNode *left = term(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS))
    {
        Token *current = parser->current_token;
        advance_parser(parser);
        ASTNode *right = term(parser);
        left = ast_init(current, left, right);
    }
    return left;
}

ASTNode *term(Parser *parser)
{
    ASTNode *left = factor(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_DIV || parser->current_token->type == TOKEN_MUL))
    {
        Token *current = parser->current_token;
        advance_parser(parser);
        ASTNode *right = factor(parser);
        left = ast_init(current, left, right);
    }
    return left;
}

ASTNode *handle_function_calls(Parser *parser, Token *current)
{
    char *function_name = current->value;
    Token *function_token = init_token(TOKEN_FUNCTION, function_name);
    ASTNode *function_call_node = ast_init(function_token, NULL, NULL);
    function_call_node->args = list_new();
    advance_parser(parser);
    while (parser->current_token && parser->current_token->type != TOKEN_RP)
    {
        if (parser->current_token && parser->current_token->type == TOKEN_COMMA)
        {
            advance_parser(parser);
            continue;
        }
        ASTNode *node = expr(parser);
        list_add(function_call_node->args, node);
    }
    advance_parser(parser);
    return function_call_node;
}

ASTNode *handle_list(Parser *parser)
{
    ASTNode *list_node = ast_init(parser->current_token, NULL, NULL);
    advance_parser(parser);
    list_node->body = list_new();
    while (parser->current_token && parser->current_token->type != TOKEN_CSB)
    {
        if (parser->current_token->type == TOKEN_COMMA)
        {
            advance_parser(parser);
            continue;
        }
        ASTNode *value = expr(parser);
        list_add(list_node->body, value);
    }
    advance_parser(parser);
    return list_node;
}

ASTNode *handle_if_condition(Parser *parser)
{
    // create if node
    ASTNode *if_node = ast_init(parser->current_token, NULL, NULL);
    advance_parser(parser);
    if_node->condition = expr(parser);
    if_node->body = block(parser);
    // create elf nodes
    ASTNode *elf_head = ast_init(NULL, NULL, NULL);
    while (parser->current_token && parser->current_token->type == TOKEN_ELF)
    {
        Token *elf_token = parser->current_token;
        advance_parser(parser);
        ASTNode *elf_node = ast_init(elf_token, NULL, NULL);
        elf_node->condition = expr(parser);
        elf_node->body = block(parser);
        elf_head->left = elf_node;
    }
    // create else node
    if (parser->current_token && parser->current_token->type == TOKEN_ELSE)
    {
        Token *else_token = parser->current_token;
        advance_parser(parser);
        ASTNode *else_node = ast_init(else_token, NULL, NULL);
        else_node->body = block(parser);
        if (elf_head->left)
        {
            if_node->left = elf_head->left;
            ASTNode *temp = elf_head->left;
            append_else_to_elf(temp, else_node);
        }
        else
            if_node->left = else_node;
    }
    free(elf_head);
    advance_parser(parser);
    return if_node;
}

ASTNode *handle_loop(Parser *parser)
{
    ASTNode *for_node = ast_init(parser->current_token, NULL, NULL);
    advance_parser(parser);
    for_node->condition = expr(parser);
    for_node->body = block(parser);
    return for_node;
}

ASTNode *factor(Parser *parser)
{
    Token *current = parser->current_token;
    ASTNode *node = NULL;
    if (!current)
        return node;

    if (current->type == TOKEN_INT ||
        current->type == TOKEN_FLOAT ||
        current->type == TOKEN_VAR ||
        current->type == TOKEN_STRING ||
        current->type == TOKEN_BOOL)
    {
        advance_parser(parser);
        if (parser->current_token && parser->current_token->type == TOKEN_LP)
            return handle_function_calls(parser, current);
        node = ast_init(current, NULL, NULL);
    }
    else if (current->type == TOKEN_LP)
    {
        advance_parser(parser);
        node = expr(parser);
        if (parser->current_token && parser->current_token->type == TOKEN_RP)
            advance_parser(parser);
        else
            throw_exception("Expected ')' after expression.");
    }
    else if (current->type == TOKEN_OSB)
        node = handle_list(parser);
    else if (current->type == TOKEN_IF)
        node = handle_if_condition(parser);
    else if ((current->type == TOKEN_FOR || current->type == TOKEN_WHILE))
        node = handle_loop(parser);
    return node;
}

List *parse(Parser *parser)
{
    advance_parser(parser);
    List *ast_nodes = program(parser);
    return ast_nodes;
}