#pragma once

#include "stdc.h"

typedef struct Lexer
{
    char c;
    unsigned int i;
    char *content;
    Set *keywords;
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
        TOKEN_SEMICOLON,
        TOKEN_COLON,
        TOKEN_LP,
        TOKEN_RP,
        TOKEN_OB,
        TOKEN_CB,
        TOKEN_OSB,
        TOKEN_CSB,
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
        TOKEN_TO,
        TOKEN_IF,
        TOKEN_ELF,
        TOKEN_ELSE,
        TOKEN_WHILE,
        TOKEN_FOR,
        TOKEN_TYPE_INT,
        TOKEN_TYPE_FLOAT,
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_BOOL,
        TOKEN_TYPE_LIST,
        TOKEN_TYPE_FUNCTION,
        TOKEN_PRINT_FUNCTION,
    } type;

    char *value;
} Token;

Lexer *initLexer(char *content, Set *keywords);

Token *initToken(int type, char *value);

List *createTokens(Lexer *lexer);