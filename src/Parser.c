#include <stdlib.h>
#include <stdio.h>

#include "headers/Lexer.h"
#include "headers/Parser.h"

ASTNode *initASTNode(Token *token, ASTNode *left, ASTNode *right)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->token = token;
    node->left = left;
    node->right = right;
    node->condition = NULL;
    node->body = newList();
    return node;
}

Parser *initParser(List *tokens)
{
    Parser *parser = malloc(sizeof(Parser));
    parser->current_token = (void *)0;
    parser->i = -1;
    parser->tokens = tokens;
    return parser;
}

void advanceParser(Parser *parser)
{
    parser->i++;
    if (parser->i < parser->tokens->size)
    {
        parser->current_token = (Token *)getFromList(parser->tokens, parser->i);
    }
}

Token *checkNextToken(Parser *parser)
{
    return (Token *)getFromList(parser->tokens, parser->i + 1);
}

ASTNode *assignExpr(Parser *parser)
{
    if (parser->current_token && parser->current_token->type != TOKEN_VAR)
    {
        fprintf(stderr, "Error: Expected a variable\n");
        exit(EXIT_FAILURE);
    }
    Token *var_name = parser->current_token;
    advanceParser(parser);
    if (parser->current_token && parser->current_token->type != TOKEN_EQUAL)
    {
        fprintf(stderr, "Error: Expected an '='\n");
        exit(EXIT_FAILURE);
    }
    ASTNode *var_name_node = initASTNode(var_name, NULL, NULL);
    Token *eq = parser->current_token;
    advanceParser(parser);
    ASTNode *expression = expr(parser);
    return initASTNode(eq, var_name_node, expression);
}

ASTNode *block(Parser *parser)
{
    ASTNode *if_expr = initASTNode(NULL, NULL, NULL);
    if (parser->current_token && parser->current_token->type == TOKEN_OB)
    {
        advanceParser(parser);
        while (parser->current_token && parser->current_token->type != TOKEN_CB)
        {
            puts(parser->current_token->value);
            ASTNode *node = expr(parser);
            if (node)
                addToList(if_expr->body, node, sizeof(ASTNode));
        }
        advanceParser(parser);
    }
    puts("asdasdasd");
    return if_expr;
}

ASTNode *expr(Parser *parser)
{
    if (parser->current_token->type == TOKEN_COMMA)
    {
        advanceParser(parser);
        return NULL;
    }
    if (
        parser->current_token &&
        (parser->current_token->type == TOKEN_TYPE_INT ||
         parser->current_token->type == TOKEN_TYPE_FLOAT ||
         parser->current_token->type == TOKEN_TYPE_STRING ||
         parser->current_token->type == TOKEN_TYPE_BOOL))
    {
        advanceParser(parser);
        return assignExpr(parser);
    }
    if (parser->current_token && parser->current_token->type == TOKEN_VAR && checkNextToken(parser)->type == TOKEN_EQUAL)
    {
        Token *var_name = parser->current_token;
        ASTNode *var_name_node = initASTNode(var_name, NULL, NULL);
        advanceParser(parser);
        Token *eq = parser->current_token;
        advanceParser(parser);
        ASTNode *expression = expr(parser);
        return initASTNode(eq, var_name_node, expression);
    }
    ASTNode *node = compExpr(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_AND || parser->current_token->type == TOKEN_OR))
    {
        Token *current = parser->current_token;
        advanceParser(parser);
        ASTNode *right = compExpr(parser);
        node = initASTNode(current, node, right);
    }
    return node;
}

ASTNode *compExpr(Parser *parser)
{
    ASTNode *node = arithExpr(parser);
    while (
        parser->current_token &&
        (parser->current_token->type == TOKEN_EE ||
         parser->current_token->type == TOKEN_GT ||
         parser->current_token->type == TOKEN_LT ||
         parser->current_token->type == TOKEN_GTE ||
         parser->current_token->type == TOKEN_LTE))
    {
        Token *current = parser->current_token;
        advanceParser(parser);
        ASTNode *right = arithExpr(parser);
        node = initASTNode(current, node, right);
    }
    return node;
}

ASTNode *arithExpr(Parser *parser)
{
    ASTNode *node = term(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS))
    {
        Token *current = parser->current_token;
        advanceParser(parser);
        ASTNode *right = term(parser);
        node = initASTNode(current, node, right);
    }
    return node;
}

ASTNode *term(Parser *parser)
{
    ASTNode *node = factor(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_DIV || parser->current_token->type == TOKEN_MUL))
    {
        Token *current = parser->current_token;
        advanceParser(parser);
        ASTNode *right = factor(parser);
        node = initASTNode(current, node, right);
    }
    return node;
}

void appendElseToElf(ASTNode *elf_node, ASTNode *else_node)
{
    if (!elf_node->right)
    {
        elf_node->right = else_node;
        return;
    }
    appendElseToElf(elf_node->right, else_node);
}
ASTNode *factor(Parser *parser)
{
    Token *current = parser->current_token;
    ASTNode *node = NULL;
    if (current && (current->type == TOKEN_INT ||
                    current->type == TOKEN_FLOAT ||
                    current->type == TOKEN_VAR ||
                    current->type == TOKEN_STRING ||
                    current->type == TOKEN_BOOL))
    {
        advanceParser(parser);
        return initASTNode(current, NULL, NULL);
    }
    else if (current && current->type == TOKEN_LP)
    {
        advanceParser(parser);
        node = expr(parser);
        if (parser->current_token && parser->current_token->type == TOKEN_RP)
        {
            advanceParser(parser);
        }
        else
        {
            fprintf(stderr, "Error: Expected ')' after expression.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (current && current->type == TOKEN_IF)
    {
        advanceParser(parser);
        ASTNode *condition_expr = expr(parser);
        ASTNode *if_expr = block(parser);
        ASTNode *else_expr = NULL;
        Token *else_token = NULL;
        ASTNode *elf_node = NULL;
        while (parser->current_token && parser->current_token->type == TOKEN_ELF)
        {
            Token *elf_token = parser->current_token;
            advanceParser(parser);
            ASTNode *condition_expr = expr(parser);
            ASTNode *elf_expr = block(parser);
            elf_node = initASTNode(elf_token, elf_expr, elf_node);
            elf_node->condition = condition_expr;
        }
        if (parser->current_token && parser->current_token->type == TOKEN_ELSE)
        {
            else_token = parser->current_token;
            advanceParser(parser);
            else_expr = block(parser);
        }
        ASTNode *else_node = initASTNode(else_token, NULL, else_expr);
        ASTNode *if_node = initASTNode(current, if_expr, NULL);
        if (elf_node)
        {
            if_node->right = elf_node;
            ASTNode *temp = elf_node;
            appendElseToElf(temp, else_node);
        }
        else
        {
            if_node->right = else_node;
        }
        if_node->condition = condition_expr;
        advanceParser(parser);
        return if_node;
    }
    return node;
}

ASTNode *parse(Parser *parser)
{
    advanceParser(parser);
    ASTNode *s = expr(parser);
}