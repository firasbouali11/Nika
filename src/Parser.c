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
    node->name = NULL;
    node->body = newList();
    node->args = newList();
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

// TODO: make block return a list of ASTNodes instead of an empty ASTNode
ASTNode *block(Parser *parser)
{
    ASTNode *placeholder = initASTNode(NULL, NULL, NULL);
    if (parser->current_token && parser->current_token->type == TOKEN_OB)
    {
        advanceParser(parser);
        while (parser->current_token && parser->current_token->type != TOKEN_CB)
        {
            ASTNode *node = expr(parser);
            if (node)
                addToList(placeholder->body, node, sizeof(ASTNode));
        }
        advanceParser(parser);
    }
    return placeholder;
}

ASTNode *functionDefinition(Parser *parser)
{
    ASTNode *node = initASTNode(parser->current_token, NULL, NULL);
    advanceParser(parser);
    if (parser->current_token && parser->current_token->type != TOKEN_VAR)
    {
        fprintf(stderr, "Error: Expected a variable\n");
        exit(EXIT_FAILURE);
    }
    node->name = parser->current_token->value;
    advanceParser(parser);
    if (parser->current_token && parser->current_token->type != TOKEN_LP)
    {
        fprintf(stderr, "Error: Expected a '('\n");
        exit(EXIT_FAILURE);
    }
    advanceParser(parser);
    while (parser->current_token && parser->current_token->type != TOKEN_RP)
    {
        if (parser->current_token->type == TOKEN_COMMA)
        {
            advanceParser(parser);
            continue;
        }
        advanceParser(parser);
        ASTNode *arg = expr(parser);
        addToList(node->args, arg, sizeof(ASTNode));
    }
    advanceParser(parser);
    ASTNode *body = block(parser);
    node->body = body->body;
    free(body);
    return node;
}

ASTNode *expr(Parser *parser)
{
    if(parser->current_token && parser->current_token->type == TOKEN_PRINT_FUNCTION){
        Token* print_token = parser->current_token; 
        advanceParser(parser);
        ASTNode* var_to_print = expr(parser); 
        ASTNode* node = initASTNode(print_token, var_to_print, NULL);
        return node;
    }
    if (parser->current_token && parser->current_token->type == TOKEN_TYPE_FUNCTION)
    {
        return functionDefinition(parser);
    }

    if (parser->current_token && parser->current_token->type == TOKEN_SEMICOLON)
    {
        advanceParser(parser);
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
        advanceParser(parser);
        return assignExpr(parser);
    }
    if (parser->current_token && parser->current_token->type == TOKEN_VAR)
    {
        int next_token_type = checkNextToken(parser)->type;
        if( next_token_type == TOKEN_EQUAL || next_token_type == TOKEN_COLON){
            Token *var_name = parser->current_token;
            ASTNode *var_name_node = initASTNode(var_name, NULL, NULL);
            advanceParser(parser);
            Token *eq = parser->current_token;
            advanceParser(parser);
            ASTNode *expression = expr(parser);
            return initASTNode(eq, var_name_node, expression);
        }
    }
    if(parser->current_token && parser->current_token->type == TOKEN_PRINT_FUNCTION){
            puts("ssss");
            Token* print_token = parser->current_token;
            advanceParser(parser);
            ASTNode* var_to_print = expr(parser);
            ASTNode* node = initASTNode(print_token, var_to_print, NULL);
            return node;
        }   
    ASTNode *node = compExpr(parser);
    while (
        parser->current_token &&
        (parser->current_token->type == TOKEN_AND ||
         parser->current_token->type == TOKEN_OR ||
         parser->current_token->type == TOKEN_TO))
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
    ASTNode *left = arithExpr(parser);
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
        left = initASTNode(current, left, right);
    }
    return left;
}

ASTNode *arithExpr(Parser *parser)
{
    ASTNode *left = term(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS))
    {
        Token *current = parser->current_token;
        advanceParser(parser);
        ASTNode *right = term(parser);
        left = initASTNode(current, left, right);
    }
    return left;
}

ASTNode *term(Parser *parser)
{
    ASTNode *left = factor(parser);
    while (parser->current_token && (parser->current_token->type == TOKEN_DIV || parser->current_token->type == TOKEN_MUL))
    {
        Token *current = parser->current_token;
        advanceParser(parser);
        ASTNode *right = factor(parser);
        left = initASTNode(current, left, right);
    }
    return left;
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
        if (parser->current_token && parser->current_token->type == TOKEN_LP)
        {
            char *function_name = current->value;
            Token *function_token = initToken(TOKEN_FUNCTION, function_name);
            ASTNode *function_call_node = initASTNode(function_token, NULL, NULL);
            advanceParser(parser);
            while (parser->current_token && parser->current_token->type != TOKEN_RP)
            {
                if (parser->current_token && parser->current_token->type == TOKEN_COMMA)
                {
                    advanceParser(parser);
                    continue;
                }
                ASTNode *node = expr(parser);
                addToList(function_call_node->args, node, sizeof(ASTNode));
            }
            advanceParser(parser);
            return function_call_node;
        }
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
    else if (current && current->type == TOKEN_OSB)
    {
        advanceParser(parser);
        List *elements = newList();
        ASTNode *list_node = initASTNode(current, NULL, NULL);
        while (parser->current_token && parser->current_token->type != TOKEN_CSB)
        {
            if (parser->current_token->type == TOKEN_COMMA)
            {
                advanceParser(parser);
                continue;
            }
            ASTNode *value = expr(parser);
            addToList(list_node->body, value, sizeof(ASTNode));
        }
        advanceParser(parser);
        return list_node;
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
            if_node->right = else_node;
        if_node->condition = condition_expr;
        advanceParser(parser);
        return if_node;
    }
    else if (current && (current->type == TOKEN_FOR || current->type == TOKEN_WHILE))
    {
        advanceParser(parser);
        ASTNode *loop_condition = expr(parser);
        ASTNode *looped_statements = block(parser);
        ASTNode *for_node = initASTNode(current, looped_statements, NULL);
        for_node->condition = loop_condition;
        return for_node;
    }
    return node;
}

ASTNode *parse(Parser *parser)
{
    advanceParser(parser);
    ASTNode *s = expr(parser);
    return s;
}