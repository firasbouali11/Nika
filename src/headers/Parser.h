#pragma once

#include "Lexer.h"
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

Parser *initParser(List *tokens);

ASTNode *assignExpr(Parser *parser);

List *block(Parser *parser);

ASTNode *functionDefinition(Parser *parser);

ASTNode *compExpr(Parser *parser);

ASTNode *arithExpr(Parser *parser);

ASTNode *term(Parser *parser);

ASTNode *expr(Parser *parser);

ASTNode *factor(Parser *parser);

ASTNode *parse(Parser *parser);