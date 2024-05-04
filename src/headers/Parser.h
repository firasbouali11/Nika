#pragma once

#include "Lexer.h"
#include "stdc.h"

typedef struct ASTNode
{
    // if conditions and loops
    struct ASTNode *condition;
    // functions
    List* args;
    char* name;
    // general purposes
    Token* token;
    struct ASTNode *left;
    struct ASTNode *right;
    List* body;
} ASTNode;

typedef struct
{
    List *tokens;
    Token *current_token;
    int i;
} Parser;

ASTNode *initASTNode(Token* token, ASTNode *left, ASTNode *right);

Parser *initParser(List* tokens);

void advanceParser(Parser *parser);

ASTNode *term(Parser *parser);

ASTNode* arithExpr(Parser* parser);

ASTNode *compExpr(Parser *parser);

ASTNode *expr(Parser *parser);

ASTNode *factor(Parser *parser);

ASTNode* parse(Parser *parser);