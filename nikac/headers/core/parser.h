#pragma once

#include "lexer.h"
#include "stdc.h"

typedef struct ASTNode
{
    // if conditions and loops
    struct ASTNode *condition;
    // functions
    List *args;
    char *name;
    // general purposes
    Token *token;
    struct ASTNode *left;
    struct ASTNode *right;
    List *body;
} ASTNode;

typedef struct
{
    List *tokens;
    Token *current_token;
    int i;
} Parser;

Parser *parser_init(List *tokens);

ASTNode *assign_expr(Parser *parser);

List *block(Parser *parser);

ASTNode *handle_function_definition(Parser *parser);

ASTNode *comp_expr(Parser *parser);

ASTNode *arith_expr(Parser *parser);

ASTNode *term(Parser *parser);

ASTNode *expr(Parser *parser);

ASTNode *logic_expr(Parser *parser);

ASTNode *factor(Parser *parser);

List *program(Parser *parser);

List *parse(Parser *parser);