#pragma once

#include "../../lib/headers/List.h"
#include "../../lib/headers/Set.h"

typedef struct Lexer
{
    char c;
    unsigned int i;
    char *content;
    Set* keywords;
} Lexer;

typedef struct Token
{
    enum
    {
        TOKEN_STRING,
        TOKEN_FLOAT,
        TOKEN_INT,
        TOKEN_BOOL,
        TOKEN_EQUAL,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_FUNCTION,
        TOKEN_VAR,
        TOKEN_COMMA,
        TOKEN_LP,
        TOKEN_RP,
        TOKEN_OB,
        TOKEN_CB,
        TOKEN_DIV,
        TOKEN_MUL,
        TOKEN_EE,
        TOKEN_GT,
        TOKEN_LT, 
        TOKEN_LTE,
        TOKEN_GTE,
        TOKEN_NE,
        TOKEN_AND,
        TOKEN_OR,
        TOKEN_IF,
        TOKEN_ELF,
        TOKEN_ELSE,
        TOKEN_WHILE,
        TOKEN_TYPE_INT,
        TOKEN_TYPE_FLOAT,
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_BOOL,
        TOKEN_TYPE_LIST,
    } type;

    char *value;
} Token;

Lexer *initLexer(char *content, Set* keywords);

void advanceLexer(Lexer *lexer);

Token *getString(Lexer *lexer);

Token *createIdentifier(Lexer *lexer);

Token *initToken(int type, char *value);

char* convertCharToString(char c);

List *createTokens(Lexer *lexer);